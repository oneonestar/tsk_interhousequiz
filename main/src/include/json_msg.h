#ifndef _JSON_MSG_H_
#define _JSON_MSG_H_

#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

void decode_question(char*, char**, char**, char**, char**, char**, char**);
void decode_json(char*, char[], char[], char[], char[]);
char *encode_json(char*, char*, char*, char*, int);

#endif
