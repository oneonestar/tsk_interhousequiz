#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "mysocket.h"

/**
 * @brief Create a listening port(socket).
 * @param port listen to which port
 * @retval >0 file descriptor for the new socket
 * @retval -1 failed to create a listening socket
 */
static int create_listening_socket( int port );

/** listening sockets */
static struct pollfd fds[200];

/** number of listening sockets + 1 */
static int nfds = 2;

int create_listening_socket( int port )
{
    int rc, on = 1;
    int listen_sd = -1;
    struct sockaddr_in addr;

    /* Create an AF_INET stream socket to receive incoming       */
    /* connections on                                            */
    listen_sd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( listen_sd < 0 )
    {
        perror( "Failed to create socket." );
        return -1;
    }
    /* Allow socket descriptor to be reuseable                   */
    rc = setsockopt( listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                     ( char * )&on, sizeof( on ) );
    if ( rc < 0 )
    {
        perror( "setsockopt() failed" );
        close( listen_sd );
        return -1;
    }
    /* Set socket to be nonblocking. All of the sockets for    */
    /* the incoming connections will also be nonblocking since  */
    /* they will inherit that state from the listening socket.   */
    rc = ioctl( listen_sd, FIONBIO, ( char * )&on );
    if ( rc < 0 )
    {
        perror( "ioctl() failed" );
        close( listen_sd );
        return -1;
    }
    /* Bind the socket                                           */
    memset( &addr, 0, sizeof( addr ) );
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl( INADDR_ANY );
    addr.sin_port        = htons( port );
    rc = bind( listen_sd,
               ( struct sockaddr * )&addr, sizeof( addr ) );
    if ( rc < 0 )
    {
        perror( "bind() failed" );
        close( listen_sd );
        return -1;
    }

    /* Set the listen back log                                   */
    rc = listen( listen_sd, 32 );
    if ( rc < 0 )
    {
        perror( "listen() failed" );
        close( listen_sd );
        return -1;
    }
    return listen_sd;
}

int send_to_port( int port, char *msg )
{
    int i, conn_count = 0;
    for ( i = 0; i < nfds; i++ )
    {
        struct sockaddr_in guest;
        socklen_t guest_len = sizeof( guest );
        getpeername( fds[i].fd, ( struct sockaddr * )&guest, &guest_len );
        char guest_ip[20];
        inet_ntop( AF_INET, &guest.sin_addr, guest_ip, sizeof( guest_ip ) );
        printf( "guest %s:%d\n", guest_ip, ntohs( guest.sin_port ) );

        if( port == ntohs( guest.sin_port ) )
        {
            printf( "%d!!", i );
            send( fds[i].fd, msg, strlen( msg ), 0 );
            conn_count++;
        }
    }
    return conn_count;
}
void poll_loop( int port , conn_callback conn_cb, timeout_callback timeout_cb, int timeout )
{
    int    sd;
    int    len, rc;
    int    new_sd = -1;
    int    end_server = FALSE, compress_array = FALSE;
    int    close_conn;
    char   buffer[80];
    int    current_size = 0, i, j;

    /* Create socket                                             */
    sd = create_listening_socket( port );

    /* Initialize the pollfd structure                           */
    memset( fds, 0 , sizeof( fds ) );

    /* Set up the initial listening socket                        */
    fds[0].fd = sd;
    fds[0].events = POLLIN;

    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    do
    {
        /* Call poll() and wait ${timeout} for it to complete.      */
        printf( "Waiting on poll()...\n" );
        rc = poll( fds, nfds, timeout );

        /* Check to see if the poll call failed.                   */
        if ( rc < 0 )
        {
            perror( "  poll() failed" );
            break;
        }

        /* Check to see if the 3 minute time out expired.          */
        if ( rc == 0 )
        {
            printf( "  poll() timed out.\n" );
            timeout_cb();
        }

        /* One or more descriptors are readable.  Need to          */
        /* determine which ones they are.                          */
        current_size = nfds;
        for ( i = 0; i < current_size; i++ )
        {
            /* Loop through to find the descriptors that returned    */
            /* POLLIN and determine whether it's the listening       */
            /* or the active connection.                             */
            if( fds[i].revents == 0 )
                continue;

            /* If revents is not POLLIN, it's an unexpected result,  */
            /* log and end the server.                               */
            if( fds[i].revents != POLLIN )
            {
                printf( "  Error! revents = %d\n", fds[i].revents );
                end_server = TRUE;
                break;
            }
            if ( i <= 1 )	//new connection to port
            {
                /* Listening descriptor is readable.                   */
                printf( "  Listening socket is readable\n" );

                /* Accept all incoming connections that are            */
                /* queued up on the listening socket before we         */
                /* loop back and call poll again.                      */
                do
                {
                    /* Accept each incoming connection. If              */
                    /* accept fails with EWOULDBLOCK, then we            */
                    /* have accepted all of them. Any other             */
                    /* failure on accept will cause us to end the        */
                    /* server.                                           */
                    new_sd = accept( fds[i].fd, NULL, NULL );
                    if ( new_sd < 0 )
                    {
                        if ( errno != EWOULDBLOCK )
                        {
                            perror( "  accept() failed" );
                            end_server = TRUE;
                        }
                        break;
                    }

                    /* Add the new incoming connection to the            */
                    /* pollfd structure                                  */
                    printf( "  New incoming connection - %d\n", new_sd );
                    fds[nfds].fd = new_sd;
                    fds[nfds].events = POLLIN;
                    nfds++;

                    /* Loop back up and accept another incoming          */
                    /* connection                                        */
                }
                while ( new_sd != -1 );
            }

            /* This is not the listening socket, therefore an        */
            /* existing connection must be readable                  */

            else
            {
                printf( "  Descriptor %d is readable\n", fds[i].fd );
                close_conn = FALSE;
                /* Receive all incoming data on this socket            */
                /* before we loop back and call poll again.            */

                do
                {
                    /* Receive data on this connection until the         */
                    /* recv fails with EWOULDBLOCK. If any other        */
                    /* failure occurs, we will close the                 */
                    /* connection.                                       */
                    rc = recv( fds[i].fd, buffer, sizeof( buffer ), 0 );
                    if ( rc < 0 )
                    {
                        if ( errno != EWOULDBLOCK )
                        {
                            perror( "  recv() failed" );
                            close_conn = TRUE;
                        }
                        if ( ( errno == EAGAIN ) || ( errno == EWOULDBLOCK ) )
                            printf( "EWOULDBLOCK\n" );
                        break;
                    }

                    /* Check to see if the connection has been           */
                    /* closed by the client                              */
                    if ( rc == 0 )
                    {
                        printf( "  Connection closed\n" );
                        close_conn = TRUE;
                        break;
                    }

                    /* Data was received                                 */
                    len = rc;
                    printf( "  %d bytes received\n", len );

                    /* Echo the data back to the client                  */
                    rc = send( fds[i].fd, buffer, len, 0 );
                    if ( rc < 0 )
                    {
                        perror( "  send() failed" );
                        close_conn = TRUE;
                        break;
                    }
                    struct sockaddr_in guest;
                    socklen_t guest_len = sizeof( guest );
                    getpeername( fds[i].fd, ( struct sockaddr * )&guest, &guest_len );
                    char guest_ip[20];
                    inet_ntop( AF_INET, &guest.sin_addr, guest_ip, sizeof( guest_ip ) );
                    int port = ntohs( guest.sin_port );
                    printf( "guest %s:%d\n", guest_ip, ntohs( guest.sin_port ) );
                    char *passing_str = malloc( sizeof( buffer ) );
                    strncpy( passing_str, buffer, len );
                    passing_str[len] = '\0';
                    conn_cb( port, passing_str );
                    break;

                }
                while( TRUE );

                /* If the close_conn flag was turned on, we need       */
                /* to clean up this active connection. This           */
                /* clean up process includes removing the              */
                /* descriptor.                                         */
                if ( close_conn )
                {
                    close( fds[i].fd );
                    fds[i].fd = -1;
                    compress_array = TRUE;
                }


            }  /* End of existing connection is readable             */
        } /* End of loop through pollable descriptors              */

        /* If the compress_array flag was turned on, we need       */
        /* to squeeze together the array and decrement the number  */
        /* of file descriptors. We do not need to move back the    */
        /* events and revents fields because the events will always*/
        /* be POLLIN in this case, and revents is output.          */
        if ( compress_array )
        {
            compress_array = FALSE;
            for ( i = 0; i < nfds; i++ )
            {
                if ( fds[i].fd == -1 )
                {
                    for( j = i; j < nfds; j++ )
                    {
                        fds[j].fd = fds[j+1].fd;
                    }
                    nfds--;
                }
            }
        }

    }
    while ( end_server == FALSE ); /* End of serving running.    */

    /* Clean up all of the sockets that are open                 */
    for ( i = 0; i < nfds; i++ )
    {
        if( fds[i].fd >= 0 )
            close( fds[i].fd );
    }
}
int create_connection( char *ip, int port )
{
    int sd, rc, on=1;
    struct sockaddr_in addr;
    sd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sd < 0 )
    {
        perror( "Failed to create socket." );
        return -1;
    }
    /* Allow socket descriptor to be reuseable                   */
    rc = setsockopt( sd, SOL_SOCKET,  SO_REUSEADDR,
                     ( char * )&on, sizeof( on ) );
    if ( rc < 0 )
    {
        perror( "setsockopt() failed" );
        close( sd );
        return -1;
    }
    memset( &addr, '0', sizeof( addr ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( port );
    inet_pton( AF_INET, ip, &addr.sin_addr );
    printf( "Connecting to - %s:%d\n", ip, port );
    rc = connect( sd, ( struct sockaddr* )&addr, sizeof( addr ) );
    if ( rc < 0 )
    {
        perror( "connect() failed" );
        close( sd );
        return -1;
    }
    printf( "Connected to - %s:%d\n", ip, port );
    fds[nfds].fd = sd;
    fds[nfds].events = POLLIN;
    nfds++;
    return 0;
}

