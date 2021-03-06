#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include "non_blocking_socket.h"

//call back to main file
void read_instruction(struct bufferevent*, void*);


void echo_event_cb(struct bufferevent *bev, short events, void *ctx)
{
	struct info *inf = ctx;
	struct evbuffer *input = bufferevent_get_input(bev);
	int finished = 0;
	if (events & BEV_EVENT_EOF) {
		size_t len = evbuffer_get_length(input);
		printf("Got a close from %s:%s.  We drained %lu bytes from it, and have %lu left.\n", inf->address, inf->port, (unsigned long)inf->total_drained, (unsigned long)len);
		finished = 1;
	}
	if (events & BEV_EVENT_ERROR) {
		printf("Got an error from %s: %s\n", inf->address, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
		finished = 1;
	}
	if (finished) {
		free(ctx);
		bufferevent_free(bev);
	}
}

void accept_connection(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *cts)
{
	char *host = (char*)malloc(sizeof(char)*1024);
	char *port = (char*)malloc(sizeof(char)*20);
	
	//only works for IPv4
	struct sockaddr_in *sin = (struct sockaddr_in *) address;
	strcpy(host, inet_ntoa(sin->sin_addr));
	sprintf(port, "%d", sin->sin_port);
	printf("Accepted connection %s:%s\n", host, port);

	//create connection information
	struct info *info1 = malloc(sizeof(struct info));
	info1->address = host;
	info1->port = port;
	info1->total_drained = 0;

	//create start reading the connection
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_instruction, NULL, echo_event_cb, info1);
	bufferevent_enable(bev, EV_READ|EV_WRITE);
}