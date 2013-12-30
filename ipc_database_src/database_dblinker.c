#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "json_string.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>

void init_track(int track[40])
{
    int index = 0;
    while(index < 40) {
        track[index] = 0;
        index++;
    }
}

bool is_track(int track[40], int item)
{
    int index = 0;
    while(index < 40) {
        if(track[index] == 0) break;
        if(track[index] == item) return true;
        index++;
    }
    return false;
}

void update_track(int track[40], int item)
{
    int index = 0;
    while(index < 40) {
        if(track[index] == item) {
            printf("%s\n", "The item is already in the track");
            break;
        }
        if(track[index] == 0) {
            printf("%s\n", "The track has been updated");
            track[index] = item;
            return;
        }
        index++;
    }
    printf("%s\n", "The track is already full");
}

void get_id(char id[3], int fd[2])
{
    close (fd[1]);
    read(fd[0], id, 3);
    printf("Child: I have receieved the id: %s\n", id);
    close(fd[0]);
}

void send_id(int fd[2])
{
    char cid[] = "1";
    close(fd[0]);
    printf("Parent: I have sent the id: %s\n", cid);
    write(fd[1], cid, sizeof(cid));
    close(fd[1]);
}

char* get_string(int fd[2])
{
    /* write through the pipe */
    char* string[600];

    close(fd[1]);
    read(fd[0], string, 600);
    close(fd[0]);

    printf("Received: %s\n", string);
    //strcmp(res_string, string);

    char** question = question_parser(string);
    char** answer = answer_parser(string);

    printf("Parent: The question is: %s\n", question[0]);
    printf("Parent: The answer is: %s\n", question[1]);
    printf("Parent: Option A = %s\n", answer[0]);
    printf("Parent: Option B = %s\n", answer[1]);
    printf("Parent: Option C = %s\n", answer[2]);
    printf("Parent: Option D = %s\n", answer[3]);
    printf("Parent: Path = %s\n", question[2]);

return string;
}

void send_string(char* json_string, int fd[2])
{
    close(fd[0]);

    printf("Child: The string is: %s\n", json_string);
    write(fd[1], json_string, strlen(json_string)+1);
    close(fd[1]);
}

void init_con(MYSQL *con)
{
    if (con == NULL) {
        printf("%s", "The connection failed, Please try again.\n");
        exit(-1);
    }

    //(localhost, name of user, password, database)
    if (mysql_real_connect(con, "localhost", "testing", "testing", "interhouse", 0, NULL, 0) == NULL) {
        printf("%s", "The connection failed, Please enter the correct password again\n");
        mysql_close(con);
        exit(-1);
    }
}

MYSQL_RES* get_result(MYSQL* con, char* query)
{
    MYSQL_RES* result;
    mysql_query(con, query);

    result = mysql_store_result(con);
    if (!result) {
        printf("%s\n", "Result");
        exit(-1);
    }
    return result;
}

void pushresult(char address[], char sendBuff[])
{
    int point;
    int trap;
    struct sockaddr_in serv_addr;

	char send[600] = "question:";
	
    strcat(send, sendBuff);
    point = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
   // memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8889);
    inet_pton(AF_INET, address, &serv_addr.sin_addr);

    connect(point, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if((trap = write(point, send, strlen(send)+1))<0) {
        printf("error\n");
    }
}

int main(int argc, char* argv[])
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[100] = "Select * from data where id = ";
    char id[3];
    int track[40];
    init_track(track);
    int intid;
    char* pushstring;
    char* pullstring[600];


    int send[2];//pipe for sending and receiveng result(json_string)
    int get[2];//pipe for sending and receiving id
    pipe(send);
    pipe(get);
    int pid = fork();

    MYSQL *con = mysql_init(NULL);
    init_con(con);
    printf("%s", "The connection succeeded\n");

    //The program starts with successful connection
    //items retreiving

    if(pid != 0) {
        get_id(id, get);
        intid = atoi(id);
        update_track(track, intid);

        strcat (query, id);//concatenation of the id and the query

        result = get_result(con, query);

        while((row = mysql_fetch_row(result))) {
            //update_data(json_string, row);
            pushstring = json_create(row[1], row[4], row[5], row[6], row[7], row[3], row[9]);
        }
        //initialization above

        printf("Created: %s\n", pushstring);
        send_string(pushstring, send);
        //testing and printing in sending function
    } else {
        send_id(get);
        strcpy(pullstring, get_string(send));
        printf("Parent: %s\n", pullstring);
        pushresult("192.168.0.102", pullstring);
    }
    mysql_free_result(result);
    mysql_close(con);

    return 0;
}
