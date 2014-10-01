#ifndef _SCORE_DB_H_
#define _SCORE_DB_H_

#include <sqlite3.h>

int score_db_get(char team);
void score_db_set(char team,char *value);
void score_db_init(int question_count);
void score_publish();

#endif
