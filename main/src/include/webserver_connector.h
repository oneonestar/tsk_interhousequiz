#ifndef _WEBSERVER_CONNECTOR_H_
#define _WEBSERVER_CONNECTOR_H_

char address_str[100];
char collection_str[50];

void webserver_update_score(char*, int);
void webserver_update_question(char*,char*,char*,char*,char*,char*);
void webserver_init(char*, char*);

#endif
