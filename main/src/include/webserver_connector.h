#ifndef _WEBSERVER_CONNECTOR_H_
#define _WEBSERVER_CONNECTOR_H_

char webserverAddr[100];
int webserverPort;

char *retrieve_questions();
void webserver_update_answer(int);
void webserver_update_score(char**, int*, int);
void webserver_update_question(char*);
void webserver_init(char*, int);

#endif
