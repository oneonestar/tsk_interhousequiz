#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <event2/event.h>

#include "include/buzzer.h"
#include "include/server.h"
#include "include/utilities.h"

#define PORT 8888

void buzzer_callback(evutil_socket_t sock, short flags, void * args)
{
	char buf[5000];  
    int ret = recv(sock, buf, 128, 0);
    buf[ret] = 0;
    
    if(ret == 0)  
    {  
        //printf("DEBUG: buzzer_callback(): read_cb connection closed\n");  
        printf("Connection to buzzer closed!\n");
        event_del(ev_read);  
        return;
    }
	printf("%s\n", buf);
	char buffer[5000];
	char buffer2[5000];
	strcpy(buffer, "buzzer:{\"");
	// for(int i = 0, i< ret, i++)
	// 	strcat(buffer, "A\":\"123\"}\n");
	sprintf(buffer2, "%c\":\"123\"}\n", house_to_char(buf[0]-49));
	strcat(buffer, buffer2);

	send_message(webServer, webPort, buffer);
}

static evutil_socket_t make_tcp_socket()  
{  
    int on = 1;  
    evutil_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
  
    evutil_make_socket_nonblocking(sock); 
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on));  
  
    return sock;  
}  


void buzzer_init(struct event_base *base)
{
	evutil_socket_t sock = make_tcp_socket();
	struct sockaddr_in serv_addr;	//addr data structure for buzzer

	//setup socket to the buzzer
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(buzzerPort));
	inet_pton(AF_INET, webServer, &serv_addr.sin_addr);
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));


	ev_read = malloc(sizeof(struct event*));
	ev_read = event_new(base, sock, (short)EV_READ|EV_PERSIST, buzzer_callback, (void*)ev_read);
	event_add(ev_read, 0);  

	printf("Reading Port for Buzzer initialized!\n");
}
