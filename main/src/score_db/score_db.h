#ifndef _SCORE_DB_H_
#define _SCORE_DB_H_
#include <sqlite3.h>

int score_get(char team);
void score_set(char team,char *value);
void score_db_init(int question_count);

#endif
