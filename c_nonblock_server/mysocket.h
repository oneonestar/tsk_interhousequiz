#ifndef MYSOCKET_H
#define MYSOCKET_H

#define TRUE             1
#define FALSE            0

typedef void (*mycallback)(int fd, char* msg);
int create_socket( int port );
int poll_loop( int port , mycallback callbacks);
#endif
