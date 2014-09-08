#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <ncurses.h>

#include "link_list.h"
#include "layout.h"
#include "score.h"
#include "buzzer.h"
#include "non_blocking_socket.h"


void server()
{
	//start score module
	score_init(0, "score_backup.dat");
	//push score to webserver
	pushScore(webServer, webPort);

	//setup UI listener
	struct event_base *base;
	struct evconnlistener *listener;
	struct sockaddr_in serv_addr;
	//setup libevent event base
	base = event_base_new();
	if(!base) {
		printf("Server: cannot open even base\n");
		return;
	}

	printf("Initializing connection to buzzer...\n");
	buzzer_init(base);

	//setup struction of the address
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(uiPort));

	//bind event to call bcak "on_accept_cb", which will then call on_read_cb()
	listener = evconnlistener_new_bind(base, on_accept_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	if(!listener) {
		printf("Server: cannot create listneer\n");
	}
	//start event loop
	event_base_dispatch(base);
}


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("usage: quiz [WebServer] [Buzzer]\n");
		return 1;
	}
	//store addresses
	strcpy(webServer, argv[1]);
	strcpy(buzzerServer, argv[2]);
	strcpy(webServer, "8888");
	strcpy(webPort, "8889");
	strcpy(uiPort, "9000");

	//initialize linked list
	listCreate(&theList);

	printf("Server starting...\n");
	server();

	return 0;
}

