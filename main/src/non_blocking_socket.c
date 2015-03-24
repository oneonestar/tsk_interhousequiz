#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#include <zlib.h>

#include <json-glib/json-glib.h>
#include "include/webserver_connector.h"

#include "include/non_blocking_socket.h" /* -> event2/util.h + event2/listener.h */
#include "include/server_cb.h"
#include "include/layout.h"
#include "include/link_list.h"

/* global variable for port and host */
char host[INET6_ADDRSTRLEN];
char port[6];	//"65535\0"

char *get_address_string(const struct Info *info)
{
	if(info->address.ss_family == AF_INET)
		evutil_inet_ntop(AF_INET, &((struct sockaddr_in*)&info->address)->sin_addr, host, INET_ADDRSTRLEN);
	else
		evutil_inet_ntop(AF_INET6, &((struct sockaddr_in6*)&info->address)->sin6_addr, host, INET6_ADDRSTRLEN);
	return host;
}
int get_port_int(const struct Info *info)
{
	if(info->address.ss_family == AF_INET)
		return ((struct sockaddr_in*)&info->address)->sin_port;
	else
		return ((struct sockaddr_in6*)&info->address)->sin6_port;
}

void on_event_cb(struct bufferevent *bev, short events, void *ctx)
{
	struct Info *inf = ctx;
	struct evbuffer *input = bufferevent_get_input(bev);
	int finished = 0;

	if(events & BEV_EVENT_EOF) {
		size_t len = evbuffer_get_length(input);
		wprintw(msg_content, "Got a close from %s:%d.  We drained %lu bytes from it, and have %lu left.\n",
			get_address_string(inf), get_port_int(inf),
			(unsigned long)inf->total_drained,
			(unsigned long)len);
		finished = 1;
	}

	if(events && BEV_EVENT_ERROR) {
		wprintw(msg_content, "Got an error from %s: %s\n", inf->address, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
		finished = 1;
	}

	if(finished) {
		node *q = listSearch(bev, theList);
		listRemove(q, theList);
		listPrint(theList);
		free(ctx);
		bufferevent_free(bev);
	}

	wrefresh(msg_content);
}

void on_accept_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *cts)
{
	struct Info *info = malloc(sizeof(struct Info));
	memcpy(&info->address, address, socklen);
	info->total_drained = 0;

	wprintw(msg_content, "Accepted connection %s:%d\n", get_address_string(info), get_port_int(info));

	/* create start reading the connection */
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	/* add connection to list */
	listAdd(bev, info, theList);
	listPrint(theList);
	wrefresh(msg_content);

	/* assign callback functions */
	bufferevent_setcb(bev, on_read_cb, NULL, on_event_cb, info);
	bufferevent_enable(bev, EV_READ|EV_WRITE);

	/* retrieve question set catalog A group 1 and push to client just connected */
	char *json_str = retrieve_questions("A:1");
	bufferevent_write(bev, json_str, strlen(json_str));
	free(json_str);
}

void on_accept_errorcb(struct evconnlistener *listener, void *ptr)
{
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Got an error %d (%s) on the listener. "
		"Shutting down.\n",
		err,
		evutil_socket_error_to_string(err));
}
