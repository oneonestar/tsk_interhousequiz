#ifndef _NON_BLOCKING_SOCKET_H_
#define _NON_BLOCKING_SOCKET_H_

#include <event2/util.h>
#include <event2/listener.h>

//setup struct info
struct Info {
	char *address;
	char *port;
	size_t total_drained;
};

static void on_event_cb(struct bufferevent*, short, void*);
void on_read_cb(struct bufferevent*, void*);
void on_accept_cb(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int, void*);

#endif
