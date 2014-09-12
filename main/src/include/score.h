#ifndef _SCORE_H_
#define _SCORE_H_

struct Score{
	int score_table[6];
	char address[50];
};

void score_init(int, char[]);
void add_score(int, int);
void minus_score(int, int);
void update_score(int, int);
void save_score(char*);
void push_score();
int get_score(char);

#endif
