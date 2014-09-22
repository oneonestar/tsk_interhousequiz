#ifndef _SCORE_H_
#define _SCORE_H_

struct Score{
	int score_table[6];
	char address[50];
};

void score_init(int, char[]);
void add_score(char, char*);
void minus_score(char, char*);
void update_score(char, char*);
void save_score(char*);
void push_score();
int get_score(char);

#endif
