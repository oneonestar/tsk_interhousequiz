#ifndef _SCORE_H_
#define _SCORE_H_
class score{
	private: 
		int score_table[6];
		char address[10];
	
	public:
		score(int initscore, char address[]);
		void add(int house, int add);
		void minus(int house, int minus);
		void update(int house, int newscore);
		int getscore(int house);
		void save();
		void scoring();
};
#endif
