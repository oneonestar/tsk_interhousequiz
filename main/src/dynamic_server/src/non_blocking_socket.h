#ifndef _NON_BLOCKING_SOCKET_H_
#define _NON_BLOCKING_SOCKET_H_
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>

#include "link_list.h"
#include "layout.h"
#include "server_cb.h"

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
