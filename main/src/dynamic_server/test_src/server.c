#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include <sys/un.h>
#include <sys/signal.h>
#include <sys/prctl.h>

#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <ncurses.h>

#include "link_list.h"
#include "server.h"
#include "layout.h"

list *theList;

void term(int signal) {
	endwin();
	printf("%d: received signal %d, end program\n", getpid(), signal);
	exit(0);
}

void on_read_cb(struct bufferevent *bev, void *ctx) {

	//reset screen if reached last line
	if(msg_h>=h-1) {
		//werase(message_box);
		msg_h = 0;
		msg_w = 1;
		box(message_box, 0, 0);
		//mvwprintw(message_box, 0, 15, "Server Activities");
		msg_h += 2;
		////wrefresh(message_box);
	}

	struct Info *inf = ctx;
	struct evbuffer *input = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(input);
	char *recvBuff;
	int i;

	if(len) {
		printf("Data of length %zu received from %s:%s\n", len, inf->address, inf->port);
		//mvwprintw(message_box, msg_h, msg_w, "Data of length %zu received from %s:%s", len, inf->address, inf->port);
		msg_h++;
		////wrefresh(message_box);
		recvBuff = (char*)malloc(sizeof(char)*(len+1));
		if(evbuffer_remove(input, recvBuff, len)<0) {
			recvBuff[len] = 0;
			printf("DEBUG: read_instruction(): copy form evbuffer failed!\n");
		}
		else {
			recvBuff[len] = 0;
		}
	}
	
	//mvwprintw(message_box, msg_h, msg_w, "Message received: %s", recvBuff);
	printf("Message received: %s\n", recvBuff);
	msg_h++;
	////wrefresh(message_box);

	//printf("msg: %s\n", recvBuff);
	node *p;
	for(i=0, p=theList->start; i<theList->size; i++, p=p->next) {
		//mvwprintw(message_box, msg_h, msg_w, "Broadcast msg to: %s:%s", p->inf->address, p->inf->port);
		printf("Broadcast msg to: %s:%s\n", p->inf->address, p->inf->port);
		msg_h++;
		////wrefresh(message_box);
		bufferevent_write(p->bev, recvBuff, strlen(recvBuff)+1);
	}
}

void on_event_cb(struct bufferevent *bev, short events, void *ctx) {
	struct Info *inf = ctx;
	struct evbuffer *input = bufferevent_get_input(bev);
	int finished = 0;
	if (events & BEV_EVENT_EOF) {
		size_t len = evbuffer_get_length(input);
		printf("Got a close from %s:%s.  We drained %lu bytes from it, and have %lu left.\n", inf->address, inf->port, (unsigned long)inf->total_drained, (unsigned long)len);
		//mvwprintw(message_box, msg_h, msg_w, "Got a close from %s:%s.  We drained %lu bytes from it, and have %lu left.", inf->address, inf->port, (unsigned long)inf->total_drained, (unsigned long)len);
		msg_h++;
		finished = 1;
	}
	if (events & BEV_EVENT_ERROR) {
		printf("Got an error from %s: %s\n", inf->address, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
		//mvwprintw(message_box, msg_h, msg_w,"Got an error from %s: %s", inf->address, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
		msg_h++;
		finished = 1;
	}
	if (finished) {
		node *q = listSearch(inf->address, inf->port, theList);
		listRemove(q, theList);
		listPrint(theList);
		free(ctx);
		bufferevent_free(bev);
	}
	//wrefresh(message_box);
}

void on_accept_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *cts) {


	char *host = (char*)malloc(sizeof(char)*1024);
	char *port = (char*)malloc(sizeof(char)*20);
	
	//only works for IPv4
	struct sockaddr_in *sin = (struct sockaddr_in *) address;
	strcpy(host, inet_ntoa(sin->sin_addr));
	sprintf(port, "%d", sin->sin_port);
	char string[100];
	printf("Accepted connection %s:%s\n", host, port);
	//mvwprintw(message_box, msg_h, msg_w, "Accepted connection %s:%s", host, port);
	msg_h++;
	//wrefresh(message_box);

	//create connection information
	struct Info *info1 = malloc(sizeof(struct Info));
	info1->address = host;
	info1->port = port;
	info1->total_drained = 0;

	//create start reading the connection
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	//add connection to list
	listAdd(bev, info1, theList);
	listPrint(theList);
	
	//assign callback functions
	bufferevent_setcb(bev, on_read_cb, NULL, on_event_cb, info1);
	bufferevent_enable(bev, EV_READ|EV_WRITE);
}

void server() {

	//setup UI listener
	struct event_base *base;
	struct evconnlistener *listener;
	struct sockaddr_in serv_addr;

	//setup libevent event base
	base = event_base_new();
	if(!base) {
		return;
	}

	//setup struction of the address
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8888);

	//bind event to call bcak "accept_connection", which will then call read_instruction()
	listener = evconnlistener_new_bind(base, on_accept_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	if(!listener) {
		printf("Server: cannot create listneer\n");
	}
	//start event loop
	event_base_dispatch(base);
}

int main(int argc, char *argv[]) {

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = term;
	sigaction(SIGINT, &action, NULL);
/*
	initscr();
	getmaxyx(stdscr, h, w);

	msg_h = 0;
	msg_w = 1;
	message_box = newwin(h, w/2, 0, 0);
	scrollok(message_box, TRUE);
	box(message_box, 0, 0);
	mvwprintw(message_box, 0, 15, "Server Activities");
	msg_h += 2;
	//wrefresh(message_box);

	cli_h = 0;
	cli_w = 1;
	client_list = newwin(h, w-w/2, 0, w/2);
	mvwprintw(client_list, 0, 1, "Client list will be shown here when avaliable.");
	scrollok(client_list, TRUE);
	box(client_list, 0, 0);
	//wrefresh(client_list);
*/	
	listCreate(&theList);
	server();
	endwin();
	return 0;
}
