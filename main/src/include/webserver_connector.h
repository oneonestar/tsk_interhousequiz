#ifndef _WEBSERVER_CONNECTOR_H_
#define _WEBSERVER_CONNECTOR_H_

char address_str[100];
char collection_str[50];

void webserver_update_answer(int);
void webserver_update_score(char**, int*, int);
void webserver_update_question(char*);
void webserver_init(char*, char*);

#endif
