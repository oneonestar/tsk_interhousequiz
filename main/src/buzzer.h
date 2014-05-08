#ifndef _BUZZER_H_
#define _BUZZER_H_

int houseToChar(int);
void buzzer_callback(evutil_socket_t sock, short flags, void * args);
void buzzer_init(struct event_base *base, char *buzzerAddress, char *buzzerPort, char *webAddress, char *webPort);
int send_message(char *address, char *port, char *msg);

#endif
