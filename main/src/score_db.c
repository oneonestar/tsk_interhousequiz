#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/score_db.h"
#include "include/server.h"
#include "include/utilities.h"

// if database file exist, continue, else create one
void score_db_init(int question_count) {

	// init
	sqlite3 *db;		// descriptor for db conn
	sqlite3_stmt *stmt;	// statement ptr
	int rc, i;		// rc=error check, i=counter
	char sql[100];		// sql = statement string
	const char *tail;	// unexec. statement

	// initialize conn
	rc = sqlite3_open_v2("score.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if(rc) {
		printf("failed to open score.db: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	// prepare statement
	strcpy(sql, "select * from score_record;");
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, &tail);
	if(rc!=SQLITE_OK) {

		// database empty, create structure
		memset(&sql, 0, sizeof(sql));
		strcpy(sql, "create table score_record(QuestionID int, Team char(1), Score int);");

		// if database created create entries for questions
		rc = sqlite3_exec(db, sql, 0, 0, 0);
		if(rc!=SQLITE_OK) {
			printf("%s\n", sqlite3_errmsg(db));
			exit(1);
		}

		// insert record
		for(i=0; i<question_count; i++) {
			memset(&sql, 0, sizeof(sql));
			sprintf(sql, "insert into score_record(QuestionID) values(%d);", i+1);
			sqlite3_exec(db, sql, 0, 0, 0);
		}
	}
	else {
		printf("Database ready\n");
	}

	// bye!
	sqlite3_close(db);
}

// set score for particular team where team=char; value = "QID:Score"
void score_db_set(char team, char *value) {

	// init variables
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc;					// error check
	char *sql = "update score_record set Score=?, Team=? where QuestionID=?";	// update question answering status
	char *sql_special = "insert into score_record(QuestionID, Team, Score) values(?, ?, ?);";
	const char *tail;			// unexec. portion of statement
	int qid, score;				// holder for question id and score

	// extract score and question ID
	sscanf(value, "%d:%d", &qid, &score);

	// open database conn
	rc = sqlite3_open_v2("score.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if(rc) {
		printf("failed to open score.db: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	if(qid==0) {
		
		sqlite3_prepare_v2(db, sql_special, strlen(sql_special), &stmt, &tail);
		sqlite3_bind_int(stmt, 1, qid);
		sqlite3_bind_text(stmt, 2, &team, 1, NULL);
		sqlite3_bind_int(stmt, 3, score);
		rc = sqlite3_step(stmt);
		if(rc!=SQLITE_DONE) {
			printf("failed to insert special scoring: %s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			exit(1);
		}
		sqlite3_finalize(stmt);
	}
	else {
		// prepare update statement
		sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
		// bind score and question id and team to prepared statement
		sqlite3_bind_int(stmt, 1, score);
		sqlite3_bind_text(stmt, 2, &team, 1, NULL);
		sqlite3_bind_int(stmt, 3, qid);
		rc = sqlite3_step(stmt);
		if(rc!=SQLITE_DONE) {
			printf("%s\n", sqlite3_errmsg(db));
			exit(1);
		}
		sqlite3_finalize(stmt);
	}

	// broadcast msg to clients
	printf("%d assigned to question %d for team %c\n", score, qid, team);
	sqlite3_close(db);
}

// retrieve score for particular team
int score_db_get(char team) {

	// init variables
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc, score;
	char *sql = "select sum(Score) from score_record where Team=?;";	// SQL to extract sum of participant
	const char *tail;							// unexec portion of sql statement

	// open db conn
	rc = sqlite3_open_v2("score.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	// prepare statement
	sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
	sqlite3_bind_text(stmt, 1, &team, 1, NULL);

	// step through result; since only one row should be returned no while loop is used
	if((rc=sqlite3_step(stmt))==SQLITE_ROW) {
		score = sqlite3_column_int(stmt,0);
	}
	sqlite3_finalize(stmt);

	// send to server
	printf("Total score for %c is %d\n", team, score);
	return score;
}

// push score to web server
void score_publish() {

	char recvBuff[100];
	sprintf(recvBuff, "score:{\"A\":\"%d\", \"D\":\"%d\", \"H\":\"%d\", \"J\":\"%d\", \"L\":\"%d\", \"M\":\"%d\"}\n", score_db_get('A'), score_db_get('D'), score_db_get('H'), score_db_get('J'), score_db_get('L'), score_db_get('M'));
	send_message(webServer, webPort, recvBuff);
	printf("Pushing updated scores to Web Server: %s\n", recvBuff);
}
