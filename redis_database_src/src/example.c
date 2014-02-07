#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis.h>

int set_question(int set, int question, char *json_str);
char* get_question(int set, int question);
int init_db();
void close_db();

redisContext *c;

int init_db()
{
    redisReply *reply;
    const char *hostname = "127.0.0.1";
    int port = 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING server */
    reply = redisCommand(c,"PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);
}

void close_db()
{
    /* Disconnects and frees the context */
    redisFree(c);
}

char* get_question(int set, int question)
{
	redisReply *reply;
	reply = redisCommand(c, "GET %d:%d", set, question);
	char *str = (char*)malloc(reply->len+1);
	strcpy(str, reply->str);
	freeReplyObject(reply);
	return str;
}


int set_question(int set, int question, char *json_str)
{
	redisReply *reply;
	reply = redisCommand(c, "SET %d:%d %s", set, question, json_str);
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);
}

int main(int argc, char **argv) {
	init_db();
	set_question(1,1,"STAR");
	printf("%s", get_question(1,1));
	close_db();
    return 0;
}
