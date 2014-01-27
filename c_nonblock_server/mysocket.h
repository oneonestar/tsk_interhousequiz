#ifndef MYSOCKET_H
#define MYSOCKET_H


#define BUZZER_PORT 8888
#define PUSH_SERVER_PORT 8889
#define GUI_PORT 8890

#define TRUE             1
#define FALSE            0

typedef void (*mycallback)(int fd, char* msg);
int create_socket( int port );
int poll_loop( int *sd_arr, int sd_arr_size , mycallback callbacks[]);

#endif
