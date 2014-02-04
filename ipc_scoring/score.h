#ifndef _SCORE_H_
#define _SCORE_H_

struct Score{
	int score_table[6];
	char address[10];
};

int server2score[2];
int score2server[2];

void kill_score();
void score_init(int, char[]);
void add(int, int);
void minus(int, int);
void update(int, int);
void save(char*);
int scoring(char *signal);
void pushScore(char*);
int get_score(char);
void change_score(char, char, int);

#endif
