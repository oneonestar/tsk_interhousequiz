#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "include/layout.h" /* -> ncurses.h */
#include "include/question_db.h"

char *get_db_question(char *cid)
{
	/* initialized db connection and io string buffers */
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc;
	char* sql;
	char *result = NULL;
	char *return_string = (char*)malloc(sizeof(char)*1000);

	/* open connection to question database */
	rc = sqlite3_open_v2("test.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

	if(rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* select question for given ID */
	sql = "SELECT data FROM question where ID=?;";
	rc = sqlite3_prepare_v2(db, sql, strlen( sql ), &stmt, NULL);
	sqlite3_bind_text(stmt, 1, cid, strlen( cid ), NULL);

	/* step through result; since only one row should be returned no while loop is used */
	if((rc=sqlite3_step(stmt)) == SQLITE_ROW) {
		result = (char*)sqlite3_column_text( stmt,0 );
		strcpy(return_string, result);
	} else {
		wprintw(msg_content, "cannot read question: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	/* finalize connection and return query result */
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return return_string;
}
