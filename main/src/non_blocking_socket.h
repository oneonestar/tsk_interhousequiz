#ifndef _NON_BLOCKING_SOCKET_H_
#define _NON_BLOCKING_SOCKET_H_
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

//setup struct info
struct info {
    char *address;
	char *port;
    size_t total_drained;
};

static void echo_event_cb(struct bufferevent*, short, void*);
void read_instruct(struct bufferevent*, void*);
void accept_connection(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int, void*);

#endif
