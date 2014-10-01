#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <event2/buffer.h>
#include <uthash.h>

#include "include/server.h"
#include "include/link_list.h"
#include "include/layout.h"
#include "include/buzzer.h"
#include "include/non_blocking_socket.h" /* -> event2/util.h + event2/listener.h */
#include "include/server_cb.h"
#include "include/score_db.h"

/* pending for removal */
#include "include/score.h"

void server()
{
	/* backward compatiblity, pending for removal */
	//start score module
	score_init(0, "score_backup.dat");
	//push score to webserver
	push_score(webServer, webPort);
	/* pending for removal */

	// setup score database and push score
	score_db_init(30);
	score_publish();

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
	strcpy(webPort, "8889");
	strcpy(buzzerServer, argv[2]);
	strcpy(uiPort, "9000");

	//initialize linked list
	listCreate(&theList);

	//setup hash table for server callback functions
	hash_table_init();

	printf("Server starting...\n");
	server();

	return 0;
}

