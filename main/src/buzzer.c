#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include "buzzer.h"

#define PORT 8888

int houseToChar(int house)
{
	switch(house) {
		case 0:
			return 'A';
			break;
		case 1:
			return 'D';
			break;
		case 2:
			return 'H';
			break;
		case 3:
			return 'J';
			break;
		case 4:
			return 'L';
			break;
		case 5:
			return 'M';
			break;
	}
	return 'z';
}

char buzzer(char *buzzerAddress, int buzzerPort, char *webServer, int webServerPort)
{
	int sock, sock1;
	int n = 0;
	char recvBuff[5];
	char sendBuff[50];
	struct sockaddr_in serv_addr;	//addr data structure for buzzer
	struct sockaddr_in serv_addr1;	//addr data structure for web server
	int flag = 0;			//indidate if the received button is first one
	char winner = 0;		//winning house in the buzz
	int i;

	//setup socket to the buzzer
	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(buzzerPort);
	inet_pton(AF_INET, buzzerAddress, &serv_addr.sin_addr);
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	//setup weocket to web server
	sock1 = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr1, '0', sizeof(serv_addr1));
	serv_addr1.sin_family = AF_INET;
	serv_addr1.sin_port = htons(webServerPort);
	inet_pton(AF_INET, webServer, &serv_addr1.sin_addr);
	connect(sock1, (struct sockaddr*)&serv_addr1, sizeof(serv_addr1));

	//clearn data structure to recv and send data
	memset(&recvBuff, 0, sizeof(recvBuff));
	memset(&sendBuff, 0, sizeof(sendBuff));

	//start reading from buzzer
	while((n = read(sock, recvBuff, sizeof(recvBuff)))>0) {
		//if it is the first button being pressed, set winner
		if(flag==0) {
			winner = houseToChar(recvBuff[0]-48);
			printf("winner %c\n", winner);
			flag++;
		}
		//when multiple houses press the button in short interval and two characters are received
		for(i=0; i<strlen(recvBuff); i++) {
			recvBuff[strlen(recvBuff)] = 0;
			sprintf(sendBuff, "buzzer:%c:%ld\n", houseToChar(recvBuff[i]-48), time(NULL));	//instruction for web server
			printf("sending: %s\n", sendBuff);
			if((n=write(sock1, sendBuff, sizeof(sendBuff)-1))<0) {	//write to web server
				printf("error\n");
			}
			printf("button %s pressed and printed\n", recvBuff);
			memset(sendBuff, 0, sizeof(sendBuff));	//clear send buffer for next send
		}
		memset(recvBuff, 0, sizeof(recvBuff));		//clear recv buffer for next read
	}
	return winner;
}
/*
int main(int argc, char *argv[])
{
	char winner = buzzer(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
	printf("Winner is %c\n", winner);
	return 0;
}
*/
