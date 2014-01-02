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
#include "ipc_scoring/score.h"
//#include "buzzer.h"

int buzzerPort = 8888;
int webPort = 8889;

void server_module(char *webServer, char *buzzingServer)
{
	struct sockaddr_in web_serv_addr;	//addr data structure for buzzer
	struct sockaddr_in buzzer_serv_addr;	//addr data structure for web server

	//setup socket to the web server
	webSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&web_serv_addr, '0', sizeof(web_serv_addr));
	web_serv_addr.sin_family = AF_INET;
	web_serv_addr.sin_port = htons(buzzerPort);
	inet_pton(AF_INET, webServer, &web_serv_addr.sin_addr);
	connect(sock, (struct sockaddr*)&web_serv_addr, sizeof(web_serv_addr));

	//setup socket to the buzzer
	webSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&buzzer_serv_addr, '0', sizeof(buzzer_serv_addr));
	buzzer_serv_addr.sin_family = AF_INET;
	buzzer_serv_addr.sin_port = htons(webPort);
	inet_pton(AF_INET, webServer, &buzzer_serv_addr.sin_addr);
	connect(sock, (struct sockaddr*)&buzzer_serv_addr, sizeof(buzzer_serv_addr));


	char recvBuff[50];
	char instruction[10], option[10], value[10], data;
	int current_question_set[6] = 0;
	int question_pointer[6] = 0;

	//start server main loop
	while(1) {

		memset(&recvBuff, 0, sizeof(recvBuff));
		memset(&instruction, 0, sizeof(instruction));
		memset(&option, 0, sizeof(option));
		memset(&value, 0, sizeof(value));

		//receive instruction from GUI


		//process instruction
		sscanf(instruction, "%s:%s:%s:%s", &instruction, &option, &data, &value);
		
		if(strcmp(instruction, "Question")==0) {
			if(strcmp(option, "Set")) {
				current_question_set[houseToIndex(data)] = atoi(value);
			}
			else if(strcmp(option, "Next")==0) {
				//read from db according to house and question pointer

				//parse JSON string
				
				//write question data to GUI

				//write to web server

				//question pointer of the set +1
				
				//buffer score to add
			}
		}
		else if(strcmp(instruction, "Score")==0) {
			if(strcmp(option, "Add")==0) {
				//add score to house who answered the last question

				//call addScore(buffered score in "Next")

				//push score to web server
			}
			else if(strcmp(option, "Update")==0) {
				//read score to update, through file/socket???

				//update each and every houses' score

				//push score to web server
			}
		}
		else if(strcmp(instruction, "Buzzer")==0) {
			if(strcmp(option, "All")==0) {
				//call buzzer function
			}
			else if(strcmp(option, "Disable")==0) {
				//read list to disable, through ???

				//call buzzer, with buttons to disable
			}
		}
		else {
			printf("Error, invalue instruction or option!\n");
		}
	}
}


int main(int argc, char *argv[])
{
	pipe(score2server);
	pipe(server2score);

	pid_t pid;

	for(int i=0; i<3; i++) {
		printf("i = %d\n", i);
		pid = fork();
		if(pid>0) {
			switch(i) {
				case 0:
					//start score module
					printf("printing score\n");
					score_init(0, "back.dat");
					scoring();
					break;
				case 1:
					//start db
					break;
				case 2:
					break;
			}
			break;
		}
	}

	//parent
	if(pid!=0) {
		printf("parent starting\n");
		server_module(argv[1], argv[2]);
	}

	return 0;
}

