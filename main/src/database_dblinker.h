#ifndef _DATABASE_DBLINKER_H_
#define _DATABASE_DBLINKER_H_
#include <mysql.h>
#include <my_global.h>

char* string_create(char *id, char *question, char* A, char* B, char *C, char *D, char *correct, char *path);
char* sql_get_result(MYSQL* con, char* cid);
MYSQL* sql_connect();

#endif
