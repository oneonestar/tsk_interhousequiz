#ifndef _DATABASE_DBLINKER_H_
#define _DATABASE_DBLINKER_H_
#include <mysql.h>
typedef MYSQL db_con;

char* db_get_result(db_con* con, char* cid);
db_con* db_connect();
void db_close(db_con* con);

#endif
