#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void score_db_init() {

	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc, row;
	char sql[100];
	const char *tail;

	rc = sqlite3_open_v2("score.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if(rc) {
		printf("failed to open score.db: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	strcpy(sql, "select * from score_record;");

	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &tail);
	if(rc!=SQLITE_OK) {
		// database empty, create structure
		memset(&sql, 0, sizeof(sql));
		strcpy(sql, "create table score_record(QuestionID int, Team char(1), Score int);");
		rc = sqlite3_exec(db, sql, 0, 0, 0);
		if(rc!=SQLITE_OK) {
			printf("%s\n", sqlite3_errmsg(db));
			exit(1);
		}
		for(int i=0; i<5; i++) {
			memset(&sql, 0, sizeof(sql));
			sprintf(sql, "insert into score_record(QuestionID) values(%d);", i);
			sqlite3_exec(db, sql, 0, 0, 0);
		}
	}
	else {
		printf("Database ready\n");
		sqlite3_finalize(stmt);
	}
	sqlite3_close(db);
}

int main(int argc, char *argv[]) {
	score_db_init();
	return 0;
}
