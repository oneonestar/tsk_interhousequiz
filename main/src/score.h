#ifndef _SCORE_H_
#define _SCORE_H_

struct Score{
	int score_table[6];
	char address[50];
};

int server2score[2];
int score2server[2];

void score_init(int, char[]);
void add(int, int);
void minus(int, int);
void update(int, int);
void save(char*);
void pushScore(char*, char*);
int get_score(char);
int house_to_index(char);
char index_to_house(int);
//void change_score(char, char, int);

#endif
