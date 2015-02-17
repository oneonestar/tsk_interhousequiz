#include <bcon.h>
#include <bson.h>
#include <stdio.h>
#include <mongoc.h>
#include <json-glib/json-glib.h>

#include "include/layout.h"	/* <-ncurses.h */
#include "include/webserver_connector.h"

void log_handler(mongoc_log_level_t log_level, const char *log_domain, const char *message, void *user_data)
{

}

void webserver_init(char *host, char *port)
{
	// initialize mongodb connection parameters
	mongoc_log_set_handler(log_handler, NULL);
	sprintf(address_str, "mongodb://%s:%s/", host, port);
}

void webserver_update_answer(int state)
{
	//setup variables
	int sock;			//socket
	int n=0;			//error check value
	char buffer[100];		//buffer
	struct sockaddr_in serv_addr;	//server address

	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, 0, sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(9001);					//port = N ie 8888
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);			//combine
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		printf("DEBUG: pushScore(): connection to web server failed\n");
	}

	sprintf(buffer, "{ \"option\": 1 }");
	send(sock, buffer, strlen(buffer)+1, 0);
	close(sock);
}

void webserver_update_question(char *question_id)
{
	//setup variables
	int sock;			//socket
	int n=0;			//error check value
	char buffer[100];		//buffer
	struct sockaddr_in serv_addr;	//server address

	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, 0, sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(9001);					//port = N ie 8888
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);			//combine
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		printf("DEBUG: pushScore(): connection to web server failed\n");
	}

	sprintf(buffer, "{ \"option\": 0, \"value\": \"%s\" }", question_id);
	send(sock, buffer, strlen(buffer)+1, 0);
	close(sock);
}

void webserver_update_score(char **name, int *new_score, int no_of_participants)
{
	//setup variables
	int sock;			//socket
	int n=0;			//error check value
	char buffer[100];		//buffer
	struct sockaddr_in serv_addr;	//server address

	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, 0, sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(9001);					//port = N ie 8888
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);			//combine
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		printf("DEBUG: pushScore(): connection to web server failed\n");
	}

	sprintf(buffer, "{ \"option\": 2, \"value\": \"%d:%d:%d:%d:%d:%d\" }", new_score[0], new_score[1], new_score[2], new_score[3], new_score[4], new_score[5]);
	send(sock, buffer, strlen(buffer)+1, 0);
	close(sock);

}

char *retrieve_questions() 
{
	//setup variables
	int sock;			//socket
	int n=0;			//error check value
	char buffer[100];		//buffer
	char recvBuff[1000];
	char *json_msg;
	struct sockaddr_in serv_addr;	//server address

	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, 0, sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(9001);					//port = N ie 8888
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);			//combine
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		printf("DEBUG: pushScore(): connection to web server failed\n");
	}

	sprintf(buffer, "{ \"option\": 3 }");
	send(sock, buffer, strlen(buffer)+1, 0);
	recv(sock, recvBuff, sizeof(recvBuff), 0);
	close(sock);

	json_msg = malloc(sizeof(char)*strlen(recvBuff)+1);
	strcpy(json_msg, recvBuff);

	return json_msg;
}
