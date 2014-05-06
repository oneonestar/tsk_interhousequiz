#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db_redis.h"

/*#include <hiredis.h>*/

int set_question(int question, char *json_str);
char* get_question(int set, int question);
int init_db();
void close_db();


db_con* db_connect()
{
    redisReply *reply;
    const char *hostname = "127.0.0.1";
    int port = 6379;
	redisContext *c;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Redis Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

	return c;
}

void db_close(db_con *con)
{
    /* Disconnects and frees the context */
    redisFree(con);
}

char* db_get_result(db_con *con, char* cid)
{	
	int set = 1;
	redisReply *reply;
	reply = redisCommand(con, "GET %s", cid);
	char *str = (char*)malloc(reply->len+1);
	if(reply->len >0)
	{
		strcpy(str, reply->str);
	}
	else
	{
		strcpy(str, "");
	}
	freeReplyObject(reply);
	return str;
}

/*char* get_question(int set, int question)*/
/*{*/
	/*redisReply *reply;*/
	/*reply = redisCommand(c, "GET %d:%d", set, question);*/
	/*char *str = (char*)malloc(reply->len+1);*/
	/*strcpy(str, reply->str);*/
	/*freeReplyObject(reply);*/
	/*return str;*/
/*}*/


/*int set_question(int question, char *json_str)*/
/*{*/
	/*int set = 1;*/
	/*redisReply *reply;*/
	/*reply = redisCommand(c, "SET %d:%d %s", set, question, json_str);*/
    /*printf("SET: %s\n", reply->str);*/
    /*freeReplyObject(reply);*/
/*}*/

/*
	init_db();
	set_question(1,1,"STAR");
	printf("%s", get_question(1,1));
	close_db();
    return 0;
	*/
