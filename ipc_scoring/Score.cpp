#include <stdio.h>
#include <iostream>
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
#define UPDATE 0
#define ADD 1
#define MIN 2
#define PORT 8889		//port for connecting to web server

using namespace std;

int server2score[2];
int score2server[2];

score::score(int initscore)
{
	//initialize scores
	for(int i=0; i<6; i++) {
		score_table[i]=initscore; 
	}
}

//return array index of a particular house
int house_to_index(char house)
{
	if(house=='A')
		return 0;
	else if(house=='D')
		return 1;
	else if(house=='H')
		return 2;
	else if(house=='J')
		return 3;
	else if(house=='L')
		return 4;
	else if(house=='M');
		return 5;
}

void score::add(int house, int add)
{
	score_table[house]+=add;
	printf("DEBUG: score::add: score_table[%d] = %d\n", house, score_table[house]);
}
	 
void score::minus(int house, int minus)
{
	score_table[house]-=minus; 
	printf("DEBUG: score::minus: score_table[%d] = %d\n", house, score_table[house]);
}
	 
void score::update(int house, int newscore)
{
	score_table[house]=newscore;
	printf("DEBUG: score::update: score_table[%d] = %d\n", house, score_table[house]);
}
	
int score::getscore(int house)
{
	printf("DEBUG: score::getscore: house = %d, score_table[] = %d\n", house, score_table[house]);
        return score_table[house];
}

//daemon of the scoring module process
void score::scoring()
{
	char signal[2];			//signal of (action)(house letter)
	char house;
	int value;

	while(1) {
		//check if the parent process is still there
		if(getppid()==1) {
			printf("parent terminated, exit...\n");
			exit(1);
		}

		//close pipe, start reading 
		close(server2score[1]);
		while(read(server2score[0], &signal, 3)>0) {
			printf("DEBUG:Scoring2: signal = %s\n", signal);
			house = signal[1];

			//update
			if(signal[0]=='U') {
				//read from parent for value to update
				close(server2score[1]);
				read(server2score[0], &value, sizeof(int));
                                int houseIndex=house_to_index(house);
                                update(houseIndex,value);
				//printf("DEBUG:update, house %c, value = %d\n", house, value);		//to be replaced by invoking method inside the object
			}
			//add
			else if(signal[0]=='A') {
				//read from parent the value to add to
				close(server2score[1]);
				read(server2score[0], &value, sizeof(int));

				//add score to defined house
				int houseIndex=house_to_index(house);
                                add(houseIndex,value);
//printf("DEBUG:add, house %c, value = %d\n", house, value);		//to be replaced by invoking mdthod inside the object
			}
			//minus
			else if(signal[0]=='M') {
				//read value to minus
				close(server2score[1]);
				read(server2score[0], &value, sizeof(int));

				//minus from defined house
                                int houseIndex=house_to_index(house);
                                minus(houseIndex,value);
				//printf("DEBUG:minus, house %c, value = %d\n", house, value);
			}
			//get
			else if(signal[0]=='G') {
				//get score
				int houseIndex=house_to_index(house);
				int score_result = getscore(houseIndex);

				//write score to parent
				close(score2server[0]);
				write(score2server[1], &score_result, sizeof(int));
			}
			else if(strcmp(signal, "ex")==0)
				//exit
				exit(0);
			printf("DEBUG: scoring: value = %d\n", value);
		}

		//clear data
		signal[0] = 0;
		value = 0;
	}
}

//request by main server
void change_score(char action, char house, int value)
{
	//command string to send over
	char cmd[3];

	//setup the string
	cmd[0] = action;
	cmd[1] = house;
	cmd[2] = 0;

	//send data over
	close(server2score[0]);
	write(server2score[1], &cmd, 3);
	write(server2score[1], &value, sizeof(int));
}

//communicate with score server to get score
int get_score(char house)
{
	//setup instruction to get score
	char signal[3] = { 'G', house };
	//append null character
	signal[2] = 0;
	//return value
	int result_score;

	//ask score server for score
	close(server2score[0]);
	write(server2score[1], &signal, 3);

	//read return value
	close(score2server[1]);
	read(score2server[0], &result_score, sizeof(result_score));

	//return requested score
	return result_score;
}

void dumpLine(FILE *stream)
{
	int c;
	while((c=getc(stream))!=EOF&&c!='\n');
}

//push score to web server for updating
void pushScore(char house, char address[])
{
	//setup variables
	int sock;			//socket
	int n=0;			//error check value
	char recvBuff[20];		//buffer
	struct sockaddr_in serv_addr;	//server address

	//get score and convert to character from integer
	memset(recvBuff, 0, sizeof(recvBuff));					//clean buffer
	sprintf(recvBuff, "score:%c:%d", house, get_score(house));
	printf("DEBUG: pushScore(): recvBuff = %s\n", recvBuff);

	//setup socket
	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, '0', sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(PORT);					//port = N ie 8888
	inet_pton(AF_INET, address, &serv_addr.sin_addr);			//combine
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));		//connect

	printf("pushScore(): score(%c) = %s\n", house, recvBuff);
	if((n=write(sock, recvBuff, sizeof(recvBuff)))<0) {			//write score to socket to python web server
		printf("error\n");
	}
}

//dummy main
int main(int argc, char *argv[])
{
	//create pipes
	pipe(server2score);
	pipe(score2server);

	//create new process
	pid_t pid = fork();

	//assign jobs
	if(pid==0) {
		//run daemon
                score *score1=new score(0);
		cout<<"constructed new obj"<<endl;
		score1->scoring();
		
	}
	else if(pid>0) {
		//testing variables
		char temp[3];
		int instruction;
		int result;
		int value = 10;
                cout<<"Hello World"<<endl;

		//waiting for instructions eg "UA", "AA"....
		printf("Enter instruction and house: ");
		while(fgets(temp, 3, stdin)!=NULL) {
			//skip NULL character
			if((int)strlen(temp) == 1) continue;

			//exit if enter 00
			if(strcmp(temp, "00")==0) {
				pushScore('A', argv[1]);
				pushScore('M', argv[1]);
				pushScore('H', argv[1]);
				pushScore('J', argv[1]);
				pushScore('L', argv[1]);
				pushScore('D', argv[1]);
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

	return 0;
}
