#include <iostream>
#include <string>
#include <stdlib>
#include <unistd.h>
#include <ctype>
#include "buzzer.h"
#include "score.h"

using namespace std;

int server2score;
int score2server;

int db2server;
int server2db;

int main(int argc, char *argv[])
{
	pipe(server2score);
	pipe(score2server);
	pipe(db2server);
	pipe(server2db);

	pid_t pid;

	for(int i=0; i<3; i++) {
		pid = fork();
		if(pid==0) {
			break;
		}
		else {
			continue;
		}
	}

	switch(i) {
		case 0:
			control();
			break;
		case 1:
			scoring();
			break;
		case 2:
			db();
			break;
	}

	return 0;
}	
