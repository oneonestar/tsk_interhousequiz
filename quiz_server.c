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
#include "ipc_database/json_string.h"

int buzzerPort = 8888;
int webPort = 8889;

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

void server_module(char *webServer, char *buzzingServer)
{
	struct sockaddr_in web_serv_addr;	//addr data structure for buzzer
	struct sockaddr_in buzzer_serv_addr;	//addr data structure for web server

	//setup socket to the web server
	int webSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&web_serv_addr, '0', sizeof(web_serv_addr));
	web_serv_addr.sin_family = AF_INET;
	web_serv_addr.sin_port = htons(buzzerPort);
	inet_pton(AF_INET, webServer, &web_serv_addr.sin_addr);
	connect(webSock, (struct sockaddr*)&web_serv_addr, sizeof(web_serv_addr));

	//setup socket to the buzzer
	int buzzerSock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&buzzer_serv_addr, '0', sizeof(buzzer_serv_addr));
	buzzer_serv_addr.sin_family = AF_INET;
	buzzer_serv_addr.sin_port = htons(webPort);
	inet_pton(AF_INET, webServer, &buzzer_serv_addr.sin_addr);
	connect(buzzerSock, (struct sockaddr*)&buzzer_serv_addr, sizeof(buzzer_serv_addr));

	//buffer variables
	char recvBuff[50];	//command from gui
	char instruction[10], option[10], value[10], data;	//for sscanf
	int intInstruction, intOption, intValue, intData;	//for parsing

	int current_question_set[6] = {0};
	int question_pointer[6] = {0};

	//start server main loop
	while(1) {
		//clean all variables before listening
		memset(&recvBuff, 0, sizeof(recvBuff));
		memset(&instruction, 0, sizeof(instruction));
		memset(&option, 0, sizeof(option));
		memset(&value, 0, sizeof(value));
		intInstruction = 0;
		intOption = 0;

		//receive instruction from GUI
		//dummy, receive from stdin
		printf("->");
		fgets(recvBuff, 49, stdin);
		recvBuff[strlen(recvBuff)] = 0;

		//process instruction
		sscanf(recvBuff, "%s %s %c %s", instruction, option, &data, value);
		intInstruction = parse_instruction(instruction);			//read instructions
		intOption = parse_option(intInstruction, option);			//read options

		//error check
		if(intInstruction==-1||intOption==-1) {
			printf("invalid instruction\n");
			continue;
		}

		//parse instructions and options
		switch(intInstruction) {
			case 0:
				//end
				printf("Exiting...\n");
				kill_score();
				exit(0);
				break;
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
				pushScore(webServer);
				break;
			case 3:
				//buzzer
				break;
		}
	}
}


int main(int argc, char *argv[])
{
	pipe(score2server);
	pipe(server2score);

	pid_t pid;

	//forking of modules
	for(int i=0; i<2; i++) {
		if((pid=fork())>0) {
			switch(i) {
				case 0:
					//start score module
					printf("printing score\n");
					score_init(0, "back.dat");
					scoring();
					break;
				/*
				case 1:
					//start db
					break;
				case 2:
					break;
					*/
			}
			break;
		}
	}

	//parent main server
	if(pid!=0) {
		printf("parent starting\n");
		server_module(argv[1], argv[2]);
	}

	return 0;
}

