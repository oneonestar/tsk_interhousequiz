#ifndef _SCORE_H_
#define _SCORE_H_

struct Score{
	int score_table[6];
	char address[10];
};

void score_init(int, char[]);
void add(int, int);
void minus(int, int);
void update(int, int);
int getscore(int);
void save(char*);
void scoring(void);

#endif
