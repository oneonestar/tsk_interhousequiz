#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "score.h"
//#include "buzzer.h"
#include "database_dblinker.h"
#include <mysql.h>
#include <my_global.h>
#include "non_blocking_socket.h"
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

char webServer[50];
char buzzerServer[50];
int buzzerPort = 8888;
int webPort = 8889;

int parse_instruction(char *instruction);
int parse_option(int instruction, char *option);

void send_message(char *address, int port, char *msg)
{
	//temp variable
	int n;

	//setup socket to the web server
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, '0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, address, &server_addr.sin_addr);
	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
		printf("fail to connect to web server\n");
	}

	if((n=send(sock, msg, strlen(msg), 0))<=0) {
		printf("cannot send to web server\n");
	}
}

//server main loop
void read_instruction(struct bufferevent *bev, void *ctx)
{
	//read from socket buffer
	char recvBuff[50];	//command from gui
	struct evbuffer *input = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(input);
	if(len) {
		evbuffer_copyout(input, recvBuff, len);
	}

	//buffer variables
	char instruction[10], option[10], value[10], data;	//for sscanf
	int intInstruction, intOption, intValue, intData;	//for parsing

	int current_question_set[6] = {0};
	int question_pointer[6] = {0};

	char buffer[500];
	
	recvBuff[strlen(recvBuff)] = 0;
	printf("received command: %s\n", recvBuff);

	//process instruction
	sscanf(recvBuff, "%s %s %c %s", instruction, option, &data, value);
	intInstruction = parse_instruction(instruction);			//read instructions
	intOption = parse_option(intInstruction, option);			//read options

	//error check
	if(intInstruction==-1||intOption==-1) {
		printf("invalid instruction\n");
	}

	//parse instructions and options
	switch(intInstruction) {
		case 0:
			//end
			printf("Exiting...\n");
			kill_score();
			exit(0);
			return;
		case 1:
			//database process
			switch(intOption) {
				case 1:
					//choose set and how many to output
					//send instruction to dabase server and read one json at a time
					//update current question pointer for respective house
					break;
				case 2:
					//read from db module, increase question pointer
					//write to gui and web server
					printf("reading question %d\n", atoi(value));
					//sprintf(buffer, "question:%s\0", fetch_question(value));
					MYSQL *con = sql_connect();
					sprintf(buffer, "question:%s", sql_get_result(con, value));
					mysql_close(con);
					printf("BUFFERERERERE:%s\n", buffer);
					send_message(webServer, webPort, buffer);
					break;
			}
			break;
		case 2:
			//score process
			switch(intOption) {
				case 1:
					//call change score
					change_score('A', data, atoi(value));
					break;
				case 2:
					//call update to overwrite
					change_score('U', data, atoi(value));
					break;
				case 3:
					//call change score to minus scores
					change_score('M', data, atoi(value));
					break;
			}
			//push score to webserver
			pushScore("127.0.0.1");
			break;
		case 3:
			//buzzer
			break;
	}
}



int parse_instruction(char *instruction)
{
	if(strcmp(instruction, "Question")==0) {
		return 1;
	}
	else if(strcmp(instruction, "Score")==0) {
		return 2;
	}
	else if(strcmp(instruction, "Buzzer")==0) {
		return 3;
	}
	else if(strcmp(instruction, "Quit")==0) {
		return 0;
	}
	return -1;
}

int parse_option(int instruction, char *option)
{
	switch(instruction) {
		case 1:
			if(strcmp(option, "Set")==0) {
				return 1;
			}
			else if(strcmp(option, "Next")==0) {
				return 2;
			}
			break;
		case 2:
			if(strcmp(option, "Add")==0) {
				return 1;
			}
			else if(strcmp(option, "Update")==0) {
				return 2;
			}
			else if(strcmp(option, "Minus")==0) {
				return 3;
			}
			break;
		case 3:
			break;
		case 0:
			return 0;
			break;
	}
	return -1;
}

void server()
{
	//setup UI listener
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
	//start score module
	printf("printing score\n");
	score_init(0, "back.dat");

	//store addresses
	strcpy(webServer, argv[1]);
	strcpy(buzzerServer, argv[2]);

	printf("parent starting\n");
	server();
	
	


	return 0;
}

