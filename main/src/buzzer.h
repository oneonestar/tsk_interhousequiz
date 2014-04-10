#ifndef _BUZZER_H_
#define _BUZZER_H_

int houseToChar(int);
void buzzerCallback(evutil_socket_t sock, short flags, void * args);
int buzzer(struct event_base *base, char *buzzerAddress, char *buzzerPort, char *webAddress, char *webPort);
int send_message(char *address, char *port, char *msg);

#endif
