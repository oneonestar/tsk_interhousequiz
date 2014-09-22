#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "include/score.h"
#include "include/utilities.h"
#include "include/server.h"

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
		if(fd==NULL) {
			printf("Initialization Error: cannot open %s\n", score.address);
			//initialize scores
			for(int i=0; i<6; i++) {
				score.score_table[i]=initscore;
			}
			return;
		}

		fgets(temp, 49, fd);
		sscanf(temp, "%d:%d:%d:%d:%d:%d", &score.score_table[0], &score.score_table[1], &score.score_table[2], &score.score_table[3], &score.score_table[4], &score.score_table[5]);
		fclose(fd);

		printf("Scores loaded...\n");
		for(int i=0; i<6; i++) {
			printf("house %c: %d\n", house_to_char(i), score.score_table[i]);
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

//save scores to file
void save_score(char *address)
{
	//open file
	FILE *file = fopen(address, "w");
	if(file==NULL) {
		printf("DEBUG: cannot save score!\n");
		return;
	}

	//prepare string to save
	char string[50];
	sprintf(string, "%d:%d:%d:%d:%d:%d", score.score_table[0], score.score_table[1], score.score_table[2], score.score_table[3], score.score_table[4], score.score_table[5]);
	fprintf(file, "%s", string);
	fclose(file);
	printf("Transction of score saved!\n");
}

void add_score(char house_char, char *add_str)
{
	int house = char_to_house(house_char);
	int add = atoi(add_str);

	score.score_table[house]+=add;
	//printf("%d added to %d, score_table[%d] = %d\n", add, house, house, score.score_table[house]);
	printf("%d added to house %c, new score is %d\n", add, house_to_char(house), score.score_table[house]);
}
	 
void minus_score(char house_char, char *minus_str)
{
	int house = char_to_house(house_char);
	int minus = atoi(minus_str);

	score.score_table[house]-=minus; 
	//printf("%d deducted to %d, score_table[%d] = %d\n", minus, house, house, score.score_table[house]);
	printf("%d deducted from house %c, new score is %d\n", minus, house_to_char(house), score.score_table[house]);
}
	 
void update_score(char house_char, char *newscore_str)
{
	int house = char_to_house(house_char);
	int newscore = atoi(newscore_str);

	score.score_table[house]=newscore;
	//printf("%d updated to %d, score_table[%d] = %d\n", newscore, house, house, score.score_table[house]);
	printf("%d updated for house %c, new score is %d\n", newscore, house_to_char(house), score.score_table[house]);
}


//communicate with score server to get score
int get_score(char house)
{
	//setup instruction to get score
	return(score.score_table[char_to_house(house)]);
}

void dumpLine(FILE *stream)
{
	int c;
	while((c=getc(stream))!=EOF&&c!='\n');
}

//push score to web server for updating
void push_score()
{
	//setup variables
	char recvBuff[100];		//buffer

	//get score and convert to character from integer
	memset(recvBuff, 0, sizeof(recvBuff));					//clean buffer
	sprintf(recvBuff, "score:{\"A\":\"%d\", \"D\":\"%d\", \"H\":\"%d\", \"J\":\"%d\", \"L\":\"%d\", \"M\":\"%d\"}\n", get_score('A'), get_score('D'), get_score('H'), get_score('J'), get_score('L'), get_score('M'));

	//send message to web server
	send_message(webServer, webPort, recvBuff);
	printf("Pushing updated scores to Web Server: %s\n", recvBuff);
}
