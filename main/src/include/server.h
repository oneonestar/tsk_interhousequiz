#ifndef _SERVER_H_
#define _SERVER_H_

//setup struct info
struct info {
	char *address;
	char *port;
	size_t total_drained;
};

//global variables to store address of buzzer and web server
char webServer[50];
char buzzerServer[50];
char buzzerPort[5];
char webPort[5];
char uiPort[5];

#endif
