#ifndef _JSON_MSG_H_
#define _JSON_MSG_H_

#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

void decode_json(char *json, char instruction[], char action[], char team[], char value[]);
char *encode_json(char *instruction, char *action, char *team, char *value, int status);

#endif
