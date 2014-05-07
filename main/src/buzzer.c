#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <event2/event.h>
#include <event2/util.h>
#include "buzzer.h"


static struct event * ev_read;
static char *webAddress, *webPort;
#define PORT 8888

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
		printf("Connection to %s:%s failed\n", address, port);
	}

	if((n=send(sock, msg, strlen(msg), 0))<=0) {
		printf("Message %s cannot be sent\n", msg);
		close(sock);
		return -1;
	}
	close(sock);
	return n;
}

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

void buzzer_callback(evutil_socket_t sock, short flags, void * args)
{
	char buf[5000];  
    int ret = recv(sock, buf, 128, 0);
    buf[ret] = 0;
    
    if(ret == 0)  
    {  
        printf("read_cb connection closed\n");  
        event_del(ev_read);  
        return;
    }
	printf("%s\n", buf);
	char buffer[5000];
	char buffer2[5000];
	strcpy(buffer, "buzzer:{\"");
	// for(int i = 0, i< ret, i++)
	// 	strcat(buffer, "A\":\"123\"}\n");
	sprintf(buffer2, "%c\":\"123\"}\n", houseToChar(buf[0]-49));
	strcat(buffer, buffer2);

	send_message(webAddress, webPort, buffer);
}

static evutil_socket_t make_tcp_socket()  
{  
    int on = 1;  
    evutil_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
  
    evutil_make_socket_nonblocking(sock); 
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on));  
  
    return sock;  
}  


void buzzer_init(struct event_base *base, char *buzzerAddress, char *buzzerPort, char *l_webAddress, char *l_webPort)
{
	evutil_socket_t sock = make_tcp_socket();
	int n = 0;
	char recvBuff[5];
	char sendBuff[50];
	struct sockaddr_in serv_addr;	//addr data structure for buzzer
	int flag = 0;			//indidate if the received button is first one
	char winner = 0;		//winning house in the buzz

	//setup socket to the buzzer
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(buzzerPort));
	inet_pton(AF_INET, buzzerAddress, &serv_addr.sin_addr);
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));


	ev_read = malloc(sizeof(struct event*));
	ev_read = event_new(base, sock, (short)EV_READ|EV_PERSIST, buzzer_callback, (void*)ev_read);
	event_add(ev_read, 0);  

	webPort = l_webPort;
	webAddress = l_webAddress;
	printf("Buzzer initialized!\n");
}
