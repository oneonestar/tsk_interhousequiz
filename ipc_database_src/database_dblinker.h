#ifdef DATABASE_DBLINKER_H
#define DATABASE_DBLINKER_H

void init_track(int track[40]);
bool is_track(int track[40], int item);
void update_track(int track[40], int item);
void get_id(wchar id[3], int fd[2]);
void send_id(int fd[2]);
wchar* get_string(int fd[2]);
void send_string(wchar* json_string, int fd[2]);
void init_con(MYSQL *con);
MYSQL_RES* get_result(MYSQL* con, wchar* query);
void pushresult(wchar address[], wchar sendBuff[]);
void database(void);

#endif
