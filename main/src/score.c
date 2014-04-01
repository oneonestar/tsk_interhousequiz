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
#define UPDATE 0
#define ADD 1
#define MIN 2
#define PORT 8889		//port for connecting to web server


//structure for score
struct Score score;

//initialize structure and load from file
void score_init(int initscore, char address[])
{
	strcpy(score.address, address);

	if(access(score.address, F_OK)!=-1) {
		printf("%s found, loading data for initialization...\n", score.address);
		char temp[50];
		FILE *fd = fopen(score.address, "r");
		fgets(temp, 49, fd);
		sscanf(temp, "%d:%d:%d:%d:%d:%d", &score.score_table[0], &score.score_table[1], &score.score_table[2], &score.score_table[3], &score.score_table[4], &score.score_table[5]);
		fclose(fd);

		printf("Scores loaded...\n");
		for(int i=0; i<6; i++) {
			printf("house %d: %d\n", i, score.score_table[i]);
		}
		printf("\n");
	}
	else {
		//initialize scores
		for(int i=0; i<6; i++) {
			score.score_table[i]=initscore; 
		}
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
	else if(house=='M')
		return 5;

	//error
	return -1;
}

//save scores to file
void save(char *address)
{
	//open file
	FILE *file = fopen(address, "w");

	//prepare string to save
	char string[50];
	sprintf(string, "%d:%d:%d:%d:%d:%d", score.score_table[0], score.score_table[1], score.score_table[2], score.score_table[3], score.score_table[4], score.score_table[5]);
	fprintf(file, "%s", string);
	fclose(file);
}

void add(int house, int add)
{
	score.score_table[house]+=add;
	printf("%d added to %d, score_table[%d] = %d\n", add, house, house, score.score_table[house]);
}
	 
void minus(int house, int minus)
{
	score.score_table[house]-=minus; 
	printf("%d deducted to %d, score_table[%d] = %d\n", minus, house, house, score.score_table[house]);
}
	 
void update(int house, int newscore)
{
	score.score_table[house]=newscore;
	printf("%d updated to %d, score_table[%d] = %d\n", newscore, house, house, score.score_table[house]);
}

//daemon of the scoring module process
void change_score(char action, char house, int value)
{
	int score_result = 0;

	//act according to instruction
	switch(action) {
		case 'U':
			update(house_to_index(house), value);
			break;
		case 'A':
			add(house_to_index(house), value);
			break;
		case 'M':
			minus(house_to_index(house), value);
			break;	
	}

	//save score to file
	save(score.address);
}

//communicate with score server to get score
int get_score(char house)
{
	//setup instruction to get score
	return(score.score_table[house_to_index(house)]);
}

void dumpLine(FILE *stream)
{
	int c;
	while((c=getc(stream))!=EOF&&c!='\n');
}

//push score to web server for updating
void pushScore(char address[], char port[])
{
	//setup variables
	int sock;			//socket
	int n=0;			//error check value
	char recvBuff[100];		//buffer
	struct sockaddr_in serv_addr;	//server address

	//get score and convert to character from integer
	memset(recvBuff, 0, sizeof(recvBuff));					//clean buffer
	sprintf(recvBuff, "score:{\"A\":\"%d\", \"D\":\"%d\", \"H\":\"%d\", \"J\":\"%d\", \"L\":\"%d\", \"M\":\"%d\"}\n", get_score('A'), get_score('D'), get_score('H'), get_score('J'), get_score('L'), get_score('M'));
	printf("DEBUG: pushScore(): recvBuff = %s\n", recvBuff);

	//setup socket
	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, '0', sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(atoi(port));					//port = N ie 8888
	inet_pton(AF_INET, address, &serv_addr.sin_addr);			//combine
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		printf("connection to web server failed\n");
	}		//connect

	if((n=write(sock, recvBuff, sizeof(recvBuff)))<0) {			//write score to socket to python web server
		printf("socket write error\n");
	}
	close(sock);
}
/*
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
		score_init(0, "backup.dat");
		printf("daemon started...\n");
		scoring();
	}
	else if(pid>0) {
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
				pushScore(argv[1]);
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
}*/
