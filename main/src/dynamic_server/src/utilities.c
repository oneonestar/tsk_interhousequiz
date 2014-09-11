#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>

#include "include/utilities.h"

//a function to send a message to whatever destination
int send_message(char *address, char *port, char *msg)
{
	//temp variable
	int n;

	//setup socket to the web server
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, '0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(port));
	inet_pton(AF_INET, address, &server_addr.sin_addr);
	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
		printf("DEBUG: send_message(): Connection to %s:%s failed\n", address, port);
		return -1;
	}

	printf("Sending message: %s to %s:%s\n", msg, address, port);
	if((n=send(sock, msg, strlen(msg), 0))<=0) {
		printf("DEBUG: send_message(): Message %s cannot be sent\n", msg);
		close(sock);
		return -1;
	}
	close(sock);
	return n;
}

//return char according to index
int house_to_char(int house)
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

//return array index of a particular house
int char_to_house(char house)
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

