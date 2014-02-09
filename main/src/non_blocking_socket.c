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
/*
void read_instruction(struct bufferevent *bev, void *ctx)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(input);
	char *data = (char*)malloc(len*sizeof(char));
	if(len) {
		evbuffer_copyout(input, data, len);
		printf("read %s\n", data);
	}
	free(data);
}
*/
void accept_connection(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *cts)
{
	//char *str = (char*)malloc(sizeof(char)*INET_ADDRSTRLEN);
	//inet_ntop(address->sa_family, &(address->sa_data), str, INET_ADDRSTRLEN);
	
	char *host = (char*)malloc(sizeof(char)*1024);
	char *port = (char*)malloc(sizeof(char)*20);
	socklen_t in_len;
	int infd;
	in_len = sizeof(address);
	int s = getnameinfo(address, sizeof(address), host, 1024, port, 20, 0);
	host[strlen(host)] = 0;
	if(s==0)
		printf("connection from %s:%s\n", host, port);
	
	struct sockaddr_in *sin = (struct sockaddr_in *) address;
	char *myaddr = inet_ntoa(sin->sin_addr);
	printf("Accepted connection %s\n", myaddr);

	//create connection information
	struct info *info1 = malloc(sizeof(struct info));
	info1->address = myaddr;
	info1->port = port;
	info1->total_drained = 0;

	//create start reading the connection
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_instruction, NULL, echo_event_cb, info1);
	bufferevent_enable(bev, EV_READ|EV_WRITE);
}
/*
void server()
{
	struct event_base *base;
	struct evconnlistener *listener;
	struct sockaddr_in serv_addr;

	base = event_base_new();
	if(!base) {
		printf("cannot open even base\n");
	}

	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(9000);

	listener = evconnlistener_new_bind(base, accept_connection, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	if(!listener) {
		printf("cannot create listneer\n");
	}

	event_base_dispatch(base);
}

int main(int argc, char *argv[])
{
	server();

	return 0;
}
*/
