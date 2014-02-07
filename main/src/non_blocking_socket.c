#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

//call back to main file
void read_instruction(struct bufferevent*, void*);

void echo_event_cb(struct bufferevent *bev, short events, void *ctx)
{

    if (events & BEV_EVENT_ERROR)

        perror("Error from bufferevent");

    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {

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
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_instruction, NULL, echo_event_cb, NULL);
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
