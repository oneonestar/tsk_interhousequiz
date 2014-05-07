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
			printf("house %c: %d\n", index_to_house(i), score.score_table[i]);
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

//return char of a particular house
char index_to_house(int house)
{
	if(house==0)
		return 'A';
	else if(house==1)
		return 'D';
	else if(house==2)
		return 'H';
	else if(house==3)
		return 'J';
	else if(house==4)
		return 'L';
	else if(house==5)
		return 'M';

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
	printf("Transction of score saved!\n");
}

void add(int house, int add)
{
	score.score_table[house]+=add;
	//printf("%d added to %d, score_table[%d] = %d\n", add, house, house, score.score_table[house]);
	printf("%d added to house %c, new score is %d\n", add, index_to_house(house), score.score_table[house]);
}
	 
void minus(int house, int minus)
{
	score.score_table[house]-=minus; 
	//printf("%d deducted to %d, score_table[%d] = %d\n", minus, house, house, score.score_table[house]);
	printf("%d deducted from house %c, new score is %d\n", minus, index_to_house(house), score.score_table[house]);
}
	 
void update(int house, int newscore)
{
	score.score_table[house]=newscore;
	//printf("%d updated to %d, score_table[%d] = %d\n", newscore, house, house, score.score_table[house]);
	printf("%d updated for house %c, new score is %d\n", newscore, index_to_house(house), score.score_table[house]);
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
	printf("Pushing updated scores to Web Server: %s\n", recvBuff);

	//setup socket
	sock = socket(AF_INET, SOCK_STREAM, 0);					//new socket stream
	memset(&serv_addr, '0', sizeof(serv_addr));				//clean address structure
	serv_addr.sin_family = AF_INET;						//internet
	serv_addr.sin_port = htons(atoi(port));					//port = N ie 8888
	inet_pton(AF_INET, address, &serv_addr.sin_addr);			//combine
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		printf("DEBUG: pushScore(): connection to web server failed\n");
	}		//connect

	if((n=write(sock, recvBuff, sizeof(recvBuff)))<0) {			//write score to socket to python web server
		printf("DEBUG: pushScore(): socket write error\n");
	}
	close(sock);
}
