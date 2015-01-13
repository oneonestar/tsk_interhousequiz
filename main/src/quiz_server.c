#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <event2/buffer.h>

#include "include/server.h"
#include "include/link_list.h"
#include "include/layout.h"
#include "include/buzzer.h"
#include "include/non_blocking_socket.h" /* -> event2/util.h + event2/listener.h */
#include "include/server_cb.h"
#include "include/score_db.h"
#include "include/layout.h"	/* ->ncurses.h */

/* signal handlers */
struct sigaction sigint_action;
struct sigaction sigwinch_action;

void server()
{
	/* initialize linked list */
	listCreate(&theList);

	/* setup score database and push score to HTML server*/
	score_db_init(30);
	score_publish();

	/* setup UI listener */
	struct event_base *base;
	struct evconnlistener *listener;

	/* setup libevent event base */
	base = event_base_new();
	if(!base) {
		wprintw(msg_content, "Server: cannot open event base\n");
		return;
	}

	/* initialize connection to buzzer server */
	wprintw(msg_content, "Initializing connection to buzzer...\n");
	buzzer_init(base);

	/* define socket data structure for IPv6 or IPv4 */
#ifndef IPv6
	struct sockaddr_in6 serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin6_family = AF_INET6;
	serv_addr.sin6_addr = in6addr_any;
	serv_addr.sin6_port = htons(atoi(uiPort));
#else
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(uiPort));
#endif

	/* bind event to call bcak "on_accept_cb", which will then call on_read_cb() */
	listener = evconnlistener_new_bind(base,
					on_accept_cb,
					NULL,
					LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
					-1,
					(struct sockaddr*)&serv_addr, sizeof(serv_addr));

	if(!listener) {
		/* cleanup ncurses */
		endwin();
		printf("Server: cannot create listener\n");
		exit(-1);
	}
	evconnlistener_set_error_cb(listener, on_accept_errorcb);

	/* start event loop */
	wrefresh(msg_content);
	event_base_dispatch(base);
}

/* window resize handler */
void sigwinch_handler(int signal)
{
	endwin();
	init_windows();
}

/* keyboard interrupt handler */
void terminate_handler(int signal)
{
	endwin();
	printf("received signal %d, terminating...\n", signal);
	exit(0);
}

/* initialize signal handlers */
void signal_handler_init()
{
	memset(&sigint_action, 0, sizeof(struct sigaction));
	sigint_action.sa_handler = terminate_handler;
	sigaction(SIGINT, &sigint_action, NULL);

	memset(&sigwinch_action, 0, sizeof(struct sigaction));
	sigwinch_action.sa_handler = sigwinch_handler;
	sigaction(SIGWINCH, &sigwinch_action, NULL);
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		printf("usage: quiz [WebServer] [Buzzer]\n");
		return 1;
	}

	/* store addresses */
	strcpy(webServer, argv[1]);
	strcpy(webPort, "8889");
	strcpy(buzzerServer, argv[2]);
	strcpy(uiPort, "9000");

	/* install signal handlers */
	signal_handler_init();

	/* setup hash table for server callback functions */
	hash_table_init();

	/* initialize web server connector */
	//initialize_web_server("localhost", "3001");

	/*/ initialize ncurses and windows */
	init_windows();

	wprintw(msg_content, "Server starting...\n");
	wrefresh(msg_content);
	server();

	return 0;
}
