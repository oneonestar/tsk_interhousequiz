#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#define UPDATE 0
#define ADD 1
#define MIN 2

//to create pipes
int score2server[2];
int server2score[2];

//global variable, testing purpose
int score[6];

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

//score server
void scoring()
{
	char signal[2];
	char house;
	int value;

	while(1) {
		if(getppid()==1) {
			printf("parent terminated, exit...\n");
			exit(1);
		}
		close(server2score[1]);
		while(read(server2score[0], &signal, 3)>0) {
			house = signal[1];
			if(signal[0]=='U') {
				close(server2score[1]);
				read(server2score[0], &value, sizeof(int));
				printf("DEBUG:update, house %c, value = %d\n", house, value);		//to be replaced by invoking method inside the object
			}
			else if(signal[0]=='A') {
				close(server2score[1]);
				read(server2score[0], &value, sizeof(int));
				printf("DEBUG:add, house %c, value = %d\n", house, value);		//to be replaced by invoking mdthod inside the object
			}
			else if(signal[0]=='M') {
				close(server2score[1]);
				read(server2score[0], &value, sizeof(int));
				printf("DEBUG:minus, house %c, value = %d\n", house, value);
			}
		}

		//clear data
		signal[0] = 0;
		value = 0;
		house = 0;
	}
}

//request by main server
void change_score(int action, char house, int value)
{
	//command string to send over
	char cmd[3];

	//determine requested action, house, and value of change
	if(action==0) {
		cmd[0] = 'U';
	}
	else if(action==1) {
		cmd[1] = 'A';
	}
	else if(action==2) {
		cmd[1] = 'M';
	}

	cmd[1] = house;
	cmd[2] = 0;

	//send data over
	close(server2score[0]);
	write(server2score[1], &cmd, 3);
	write(server2score[1], &value, sizeof(int));
}

int main(void)
{
	//create pipes
	pipe(server2score);
	pipe(score2server);

	//create new process
	pid_t pid = fork();

	//assign jobs
	if(pid>0) {
		//run daemon
		scoring();
	}
	else if(pid==0) {
		//testing variables
		char temp[3];
		int value = 10;
		//call IPC module to communicate with score server
		change_score(UPDATE, 'D', 10);
		change_score(ADD, 'A', 5);
		change_score(MIN, 'M', 15);
	}

	return 0;
}
