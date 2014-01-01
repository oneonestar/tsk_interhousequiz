#ifdef DATABASE_DBLINKER_H
#define DATABASE_DBLINKER_H

void init_track(int track[40]);
bool is_track(int track[40], int item);
void update_track(int track[40], int item);
void get_id(char id[3], int fd[2]);
void send_id(int fd[2]);
char* get_string(int fd[2]);
void send_string(char* json_string, int fd[2]);
void init_con(MYSQL *con);
MYSQL_RES* get_result(MYSQL* con, char* query);
void pushresult(char address[], char sendBuff[]);
void database(int argc, char* argv[]);

#endif
