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

void server_module(char *webServer, char *buzzingServer)
{
	//testing variables
	char temp[3];
	int instruction;
	int result;
	int value = 10;
	//waiting for instructions eg "UA", "AA"....
	printf("Enter instruction and house: ");
	while(fgets(temp, 3, stdin)!=NULL) {
		//skip NULL character
		if((int)strlen(temp) == 1) continue;
			//exit if enter 00
		if(strcmp(temp, "00")==0) { 
			pushScore(webServer);
			//tell child to stop
			close(server2score[0]);
			write(server2score[1], "ex", sizeof("ex"));
			exit(0);
		}

		//read for change in value
		printf("Enter value: ");
		scanf("%d", &value);
		//library call of change_score();
		change_score(temp[0], temp[1], value);
		printf("main(): score of %c is %d\n", temp[1], get_score(temp[1]));
		//reset variables
		temp[0] = 0;
		instruction = -1;
		printf("Enter instruction and house, 00 to quit: ");
	}

	//exit returning correct
	exit(0);
}

int main(int argc, char *argv[])
{
	pipe(score2server);
	pipe(server2score);

	pid_t pid;
	for(int i=0; i<3; i++) {
		if(pid==0) {
			switch(i) {
				case 0:
					//start score module
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
		else {
			continue;
		}
	}

	//parent
	if(pid!=0) {
		server_module(argv[1], argv[2]);
	}

	return 0;
}

