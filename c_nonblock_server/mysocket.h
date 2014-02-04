#ifndef MYSOCKET_H
#define MYSOCKET_H

#define TRUE             1
#define FALSE            0

typedef void (*conn_callback)(int fd, char* msg);
typedef void (*timeout_callback)();
int create_socket( int port );
//Timeout base on millisecond
int poll_loop( int port , conn_callback conn_cb, timeout_callback timeout_cb, int timeout);
int send_to_port(int port, char* msg);
#endif
