#ifndef _BUZZER_H_
#define _BUZZER_H_

void buzzer_callback(evutil_socket_t sock, short flags, void * args);
void buzzer_init(struct event_base *base);
static evutil_socket_t make_tcp_socket();

#endif
