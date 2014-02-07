#ifndef MYSOCKET_H
#define MYSOCKET_H

#define TRUE             1
#define FALSE            0

/** Callback function prototype for receiving msg. */
typedef void ( *conn_callback )( int port, char* msg );

/** Callback function prototype for poll() timeout. */
typedef void ( *timeout_callback )();

/**
 * @brief Loop waiting any connection, msg or timeout then callback.
 * @param port listening to which port
 * @param conn_cb callback after receiving a msg
 * @param timeout_cb callback after timeout
 * @param timeout timeout the listen in millisecond
 */
void poll_loop( int port , conn_callback conn_cb, timeout_callback timeout_cb, int timeout );


/**
 * @brief Send message to connecting sockets according to the distination port.
 * @param port the destination port
 * @param msg msg needed to send out
 * @retval number of connections sended by this function
 */
int send_to_port( int port, char* msg );

/**
 * @brief Start a socket to target IP:Port
 * @param ip target ip address (eg."127.0.0.1")
 * @param port target port address
 * @retval 0 success
 * @retval <0 connection fail
 */
int create_connection( char *ip, int port );

#endif
