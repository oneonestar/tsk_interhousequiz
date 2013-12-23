#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

//pipes
int database2server[2], server2database[2];

//server side
void question_fetch(char question_id[5], char question[150], char option[][50], char *ans)
{
	//give data to sizeof
	char opt[4][50];

	//test
	printf("DEBUG:question_fetch(): received data question_id %s\n",question_id);

	//declare signal
	int signal;

	//signal 0 = get question
	signal = 0;
	close(server2database[0]);
	write(server2database[1], &signal, sizeof(int));						//server to database, signal
	write(server2database[1], question_id, strlen(question_id)+1);					//server to database, question ID

	//read result
	close(database2server[1]);
	read(database2server[0], question, 150);
	printf("DEBUG:question_fetch(): question = %s\n", question);

	//signal 1 = get option set
	signal = 1;
	close(server2database[0]);
	write(server2database[1], &signal, sizeof(int));
	write(server2database[1], question_id, strlen(question_id)+1);

	//read restult
	close(database2server[1]);
	read(database2server[0], option, sizeof(opt));

	printf("DEBUG:question_fetch(): ans %s, %s, %s, %s\n", option[0], option[1], option[2], option[3]);

	//signal 2 = get correct answer
	signal = 2;
	close(server2database[0]);
	write(server2database[1], &signal, sizeof(int));
	write(server2database[1], question_id, strlen(question_id)+1);

	//read result
	close(database2server[1]);
	read(database2server[0], ans, sizeof(char));
	printf("DEBUG:question_fetch(): correct ans = %c\n", ans);

	//signal 3 = out signal
	signal = 3;
	close(server2database[0]);
	write(server2database[1], &signal, sizeof(int));
	write(server2database[1], question_id, strlen(question_id)+1);

}

//database side
void question_send()
{
	//initialize variables
	char question_id[6], option[4][50], ans;
	char question[150];
	int signal;

	//test data for DEBUG
	ans = 'C';
	strcpy(question, "Who study in UST?");
	strcpy(option[0], "Steven");
	strcpy(option[1], "Charlie");
	strcpy(option[2], "Alfred");
	strcpy(option[3], "Jacky");

	while(1) {
		//check if parent is terminated
		if(getppid()==1) {
			printf("perent terminated, exiting...\n");
			exit(1);
		}

		//close write end
		close(server2database[1]);
		while(read(server2database[0], &signal, sizeof(int))>0) {
			//read question_id
			read(server2database[0], question_id, sizeof(question_id));
			printf("DEBUG:question_send(): question id = %s\n", question_id);

			//determine signal
			//send question
			if(signal==0) {
				printf("DEBUG:question_send(): signal = %d\n", signal);
				close(database2server[0]);
				write(database2server[1], &question, strlen(question)+1);
			}

			//send option
			else if(signal==1) {
				printf("DEBUG:question_send(): signal = %d\n", signal);
				close(database2server[0]);
				write(database2server[1], &option, sizeof(option));
			}

			//send correct answer
			else if(signal==2) {
				printf("DEBUG:question_send(): signal = %d\n", signal);
				close(database2server[0]);
				write(database2server[1], &ans, sizeof(ans));
			}

			//out
			else if(signal==3) {
				//clean variables
				//question[0] = 0;
				question_id[0] = 0;
				signal = -1;
			}
		}
	}
}

//flush stdin
void dumpLine(FILE *stream)
{
	int c;
	while((c=getc(stream))!=EOF&&c!='\n');
}

int main(void)
{
	//open two pipes
	pipe(database2server);
	pipe(server2database);

	//generate new process
	pid_t pid = fork();

	//allocate tasks
	if(pid>0) {
		//database side
		question_send();
	}
	else if(pid==0) {
		//main server side

		//initialize variables
		char temp[5], question[150], option[4][50], ans;
		while(1) {
			//ask for question ID
			printf("\nDEBUG:Enter question id: ");
			fgets(temp, 5, stdin);
			temp[strlen(temp)] = 0;

			//call for data
			question_fetch(temp, &question, &option, &ans);

			//debug
			printf("DEBUG:main(): question = %s\n", question);
			printf("DEBUG:main(): option = %s, %s, %s, %s\n", option[0], option[1], option[2], option[3]);
			printf("DEBUG:main(): answer = %c\n", ans);
			temp[0] = 0;
		}
	}

	//bye
	return 0;
}
