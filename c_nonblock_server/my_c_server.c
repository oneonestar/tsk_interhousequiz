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

void callback_thisfunc(int fd, char* msg)
{
	printf("callback:\n");
	printf("message: %s\n\n", msg);
	/* Add your code here */
}

int main( int argc, char *argv[] )
{
	int port = 9000;
    poll_loop( port, callback_thisfunc );
    return 0;
}
