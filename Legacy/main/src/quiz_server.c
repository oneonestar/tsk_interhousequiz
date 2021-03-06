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
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "score.h"
#include "buzzer.h"
#include "non_blocking_socket.h"
#include "db_redis.h"

//global variables to store address of buzzer and web server
char webServer[50];
char buzzerServer[50];
char buzzerPort[] = "8888";
char webPort[] = "8889";
char uiPort[] = "9000";

//declaration
int parse_instruction(char *instruction);
int parse_option(int instruction, char *option);

//server main loop and call back function to parse instruction from telnet
void read_instruction(struct bufferevent *bev, void *ctx)
{
	//read from socket buffer
	//evbuffer created to get input from buffer, obtain lenth of data in buffer with evbuffer_get_length()
	//allot memory and call evbuffer_remove() to copy data into recvBuffer and drain data inside buffer
	char *recvBuff;
	struct evbuffer *input = bufferevent_get_input(bev);	//read the buffer
	struct info *inf = ctx;									//get information about the connection
	size_t len = evbuffer_get_length(input);				//get length
	if(len) {
		printf("\nData of length %zu received from %s:%s\n", len, inf->address, inf->port);
		recvBuff = (char*)malloc(sizeof(char)*(len+1));
		if(evbuffer_remove(input, recvBuff, len)<0) {
			recvBuff[len] = 0;
			printf("DEBUG: read_instruction(): copy form evbuffer failed!\n");
		}
		else {
			recvBuff[len] = 0;
		}
		
	}

	//parsing variables
	//instruction=command catag, option=action to be taken; value=a char value; data=an int value
	char instruction[10], option[10], value[100], data;	//for sscanf
	int intInstruction, intOption, intValue, intData;	//for parsing

	//a buffer for storing returned string from functions
	char buffer[5000];
	
	//DEBUG
	printf("Command Received: %s\n", recvBuff);

	//process instruction
	sscanf(recvBuff, "%s %s %c %s", instruction, option, &data, value);
	intInstruction = parse_instruction(instruction);			//read instructions
	intOption = parse_option(intInstruction, option);			//read options

	//free recvBuff after use
	free(recvBuff);
	
	//error check
	if(intInstruction==-1||intOption==-1) {
		printf("invalid instruction!\n");
	}

	//parse instructions and options
	switch(intInstruction) {

		case 0:
			//end
			printf("Exiting...\n");
			exit(0);
			break;
		case 1:
			//database process
			switch(intOption) {
				case 1:
					//to read a question from database
					printf("Reading Question: %s\n", value);
					
					//procedure to get json string from database module
					db_con *con = db_connect();									//initiate mysql connection
					sprintf(buffer, "question:%s", db_get_result(con, value));	//get question with question ID and store in buffer
					db_close(con);											//close the connection
					
					//send message to webserver to show question
					printf("Sending Question to Web Server: ");
					send_message(webServer, webPort, buffer);
					break;
			}
			break;
		case 2:
			//score process
			switch(intOption) {
				case 1:
					//call change score
					add(house_to_index(data), atoi(value));
					break;
				case 2:
					//call update to overwrite
					update(house_to_index(data), atoi(value));
					break;
				case 3:
					//call change score to minus scores
					minus(house_to_index(data), atoi(value));
					break;
			}

			//push score to webserver
			save("score_backup.dat");
			pushScore(webServer, webPort);
			break;

		case 3:
			//Buzzer
			switch(intOption) {
				//being able to enter this block implies that the 'option' variable is safe to use
				case 0:
					printf("Requesting to reset Buzzer\n");
					send_message(buzzerServer, buzzerPort, option);
					break;

				case 1:
					printf("Requesting to enable Buzzer\n");
					send_message(buzzerServer, buzzerPort, option);
					break;

				case 2:
					printf("Requesting to disable Buzzer\n");
					send_message(buzzerServer, buzzerPort, option);
					break;
			}
			
			break;

		case 4:
			//answer
			printf("Requesting to show answer: ");
			send_message(webServer, webPort, "answer:{}");
			break;

		case 5:
			switch(intOption) {
			
				case 1:
					printf("Requesting to show the score panel: ");
					send_message(webServer, webPort, "ui:{\"score\":\"show\"}");
					break;
			
				case 2:
					printf("Requesting to hide the score panel: ");
					send_message(webServer, webPort, "ui:{\"score\":\"hide\"}");
					break;
			}

			break;
	}
	
	//clear information
	memset(&recvBuff, 0, sizeof(recvBuff));
	memset(&buffer, 0, sizeof(buffer));
	intInstruction = 0;
	intOption = 0;
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
	else if(strcmp(instruction, "Answer")==0) {
		return 4;
	}
	else if(strcmp(instruction, "UI")==0) {
		return 5;
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
			if(strcmp(option, "Next")==0) {
				return 1;
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
			if(strcmp(option, "0")==0) {
				return 0;
			}
			else if(strcmp(option, "1")==0) {
				return 1;
			}
			else if(strcmp(option, "2")==0) {
				return 2;
			}
			break;

		case 4:
			return 0;
			break;

		case 5:
			if(strcmp(option, "Show")==0) {
				return 1;
			}
			else if(strcmp(option, "Hide")==0) {
				return 2;
			}
			break;

		case 0:
			return 0;
			break;
	}
	return -1;
}

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
	buzzer_init(base, buzzerServer, buzzerPort, webServer, webPort);

	//setup struction of the address
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(uiPort));

	//bind event to call bcak "accept_connection", which will then call read_instruction()
	listener = evconnlistener_new_bind(base, accept_connection, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

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

	printf("Server starting...\n");
	server();

	return 0;
}

