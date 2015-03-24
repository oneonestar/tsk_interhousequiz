#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <json-glib/json-glib.h>

#include "include/layout.h"	/* <-ncurses.h */
#include "include/webserver_connector.h"

void webserver_rpc(char *host, int port, int service, char *value, char **feedback)
{
	//setup variables
	int sock;			//socket
	char buffer[100];		//buffer
	char *recvBuff;
	int length;
	struct sockaddr_in serv_addr;	//server address

	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, 0, sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(port);					//port = N ie 8888
	inet_pton(AF_INET, host, &serv_addr.sin_addr);			//combine
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		wprintw(msg_content, "DEBUG: RPC to webserver failed!\n");
		wrefresh(msg_content);
		return;
	}

	if(value!=NULL) {
		sprintf(buffer, "{ \"option\": %d, \"value\": \"%s\" }", service, value);
	}
	else {
		sprintf(buffer, "{ \"option\": %d }", service);
	}
	wprintw(msg_content, "rpc(): %s\n", buffer);
	wrefresh(msg_content);

	send(sock, buffer, strlen(buffer)+1, 0);
	if(feedback!=NULL) {
		// check size of string to be returned and allot memory
		recv(sock, &length, sizeof(int), 0);
		recvBuff = malloc(sizeof(char)*length+1);
		wprintw(msg_content, "Msg size: %d\n", length);
		wrefresh(msg_content);

		// recv actual content
		recv(sock, recvBuff, sizeof(char)*length+1, 0);
		*feedback = malloc(sizeof(char)*strlen(recvBuff)+1);
		strcpy(*feedback, recvBuff);
	}
	close(sock);
}

void webserver_init(char *host, int port)
{
	strcpy(webserverAddr, host);
	webserverPort = port;
}

void webserver_update_answer(int state)
{
	webserver_rpc(webserverAddr, webserverPort, 1, NULL,NULL);
}

void webserver_update_question(char *question_id)
{
	webserver_rpc(webserverAddr, webserverPort, 0, question_id, NULL);
}

void webserver_update_score(char **name, int *new_score, int no_of_participants)
{
	char scores[100];
	sprintf(scores, "%d:%d:%d:%d:%d:%d", new_score[0], new_score[1], new_score[2], new_score[3], new_score[4], new_score[5]);
	webserver_rpc(webserverAddr, webserverPort, 2, scores, NULL);
}

char *retrieve_questions(char *id) 
{
	char *recvBuff;
	webserver_rpc(webserverAddr, webserverPort, 3, id, &recvBuff);
	wprintw(msg_content,"%s",recvBuff);
	wrefresh(msg_content);
	return recvBuff;
}
