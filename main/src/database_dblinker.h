#ifndef DATABASE_DBLINKER_H
#define DATABASE_DBLINKER_H
#include <mysql.h>
char* string_create(char *id, char *question, char* A, char* B, char *C, char *D, char *correct, char *path);
char* sql_get_result(MYSQL* con, char* cid);
MYSQL* sql_connect();

#endif
