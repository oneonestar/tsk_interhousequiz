#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "mysocket.h"

void callback_8888(int fd, char* msg)
{
	printf("callback_8888:\n");
	printf("message: %s\n\n", msg);
	/* Add your code here */
}

void callback_8889(int fd, char* msg)
{
	printf("callback_8889:\n");
	printf("message: %s\n\n", msg);
	/* Add your code here */
}

void callback_8890(int fd, char* msg)
{
	printf("callback_8890:\n");
	printf("message: %s\n\n", msg);
	/* Add your code here */
}

int main( int argc, char *argv[] )
{
	/* Some socket setup */
    int arr[] = {BUZZER_PORT, PUSH_SERVER_PORT, GUI_PORT};
	mycallback callbacks[] = {callback_8888, callback_8889, callback_8890};
    int length = sizeof( arr )/sizeof(int);

    int *sd_arr = ( int * )malloc( sizeof( int )*length );
    int i;
    for ( i=0; i<length; i++ )
        sd_arr[i] = create_socket( arr[i] );

	/*start looping*/
    poll_loop( sd_arr, length, callbacks );
	
    return 0;
}
