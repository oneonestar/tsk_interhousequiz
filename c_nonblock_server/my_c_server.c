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

void mytimeout_callback()
{
	printf("timeout\n");
	send_to_port(8887, "star");
	/* Add your code here */
}

void myconn_callback(int port, char* msg)
{
	printf("callback:\n");
	printf("message: %s\n\n", msg);
	/* Add your code here */
}
int main( int argc, char *argv[] )
{
	int port = 9000;
    poll_loop( port, myconn_callback, mytimeout_callback , 3*1000); //3 sec timeout 
    return 0;
}
