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

typedef struct stucture
{
    //string question, answer, oa, ob, oc, od;
    char question[50];
    char answer[200];
    char option[4][50];
    char path[30];
} data;

void init_data(data* a)
{
    strcpy(a->question, "\n");
    strcpy(a->answer, "\n");

    int index;
    for (index = 0; index < 4; index++);
    {
        strcpy(a->option[index], "\n");
    }

    strcpy(a->path, "\n");
	printf("parent\n");
}

void init_track(int track[40])
{
    int index = 0;
    while(index < 40)
    {
        track[index] = 0;
        index++;
    }
}

void update_data(data* a, MYSQL_ROW row)
{
    strcpy(a->question, (char*)row[1]);
    strcpy(a->answer, (char*)row[3]);
    if (strcmp(row[2], "MC") == 0)
    {
        int index;
        for (index = 0; index < 4; index++)
        {
            strcpy(a->option[index], (char*)row[index+4]);
        }
    }
    if (strcmp(row[8], "Y") == 0)
        strcpy(a->path, (char*)row[9]);
}

bool is_track(int track[40], int item)
{
    int index = 0;
    while(index < 40)
    {
        if(track[index] == 0) break;
        if(track[index] == item) return true;
        index++;
    }
    return false;
}

void update_track(int track[40], int item)
{
    int index = 0;
    while(index < 40)
    {
        if(track[index] == item)
        {
            printf("%s\n", "The item is already in the track");
            break;
        }
        if(track[index] == 0)
        {
            printf("%s\n", "The track has been updated");
            track[index] = item;
            return;
        }
        index++;
    }
    printf("%s\n", "The track is already full");
}

void get_id(char id[3], int fd[2], int pid)
{
    char cid[] = "1";
    if (pid != 0)
    {
        close (fd[1]);
        read(fd[0], id, 3);
        printf("Child: I have receieved the id: %s\n", id);
        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        printf("Parent: I have sent the id: %s\n", cid);
        write(fd[1], cid, sizeof(cid));
        close(fd[1]);
    }
}

void sending(char* json_string, int fd[2], int pid)
{
	printf("The pid is %i\n", pid);
    /* write through the pipe */
    if(pid == 0)
    {
        //data* parent;

	//init_data(parent);

        char* string[600];

        close(fd[1]);
        read(fd[0], string, 600);
	close(fd[0]);

        printf("Received: %s\n", string);
	
//	data* parent;
//	init_data(parent);
	char** question = question_parser(string);
	char** answer = answer_parser(string);
/*	strcpy(parent->question, *gosh);
        strcpy(parent->answer, question_parser(string)[1]);
        int index;
        for (index=0; index<4; index++)
        {
            strcpy(parent->option[index], json_parse_array(string, index));
        }
	strcpy(parent->path, question_parser(string)[2]);
*/
        printf("Parent: The question is: %s\n", question[0]);
        printf("Parent: The answer is: %s\n", question[1]);
        printf("Parent: Option A = %s\n", answer[0]);
        printf("Parent: Option B = %s\n", answer[1]);
        printf("Parent: Option C = %s\n", answer[2]);
        printf("Parent: Option D = %s\n", answer[3]);
        printf("Parent: Path = %s\n", question[2]);
	//close(fd[0]);
    }
    else
    {
        close(fd[0]);

        printf("Child: The string is: %s\n", json_string);
        write(fd[1], json_string, strlen(json_string)+1);
        close(fd[1]);
    }
}


void init_con(MYSQL *con)
{
    if (con == NULL)
    {
        printf("%s", "The connection failed, Please try again.\n");
        exit(-1);
    }

    //(localhost, name of user, password, database)
    if (mysql_real_connect(con, "localhost", "testing", "testing", "interhouse", 0, NULL, 0) == NULL)
    {
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
    if (!result)
    {
        printf("%s\n", "Result");
        exit(-1);
    }
    return result;
}

int main()
{
    data* a;
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[100] = "Select * from data where id = ";
    char id[3];
    int track[40];
    init_track(track);
    int intid;
    char* json_string;

    int send[2];
    int get[2];
    pipe(send);
    pipe(get);
    int pid = fork();

    MYSQL *con = mysql_init(NULL);
    init_con(con);
    printf("%s", "The connection succeeded\n");

    //The program starts with successful connection
    //items retreiving
    get_id(id, get, pid);

    if(pid != 0)
    {
        intid = atoi(id);
        update_track(track, intid);

        strcat (query, id);//concatenation of the id and the query

        result = get_result(con, query);

        while((row = mysql_fetch_row(result)))
        {
            //update_data(json_string, row);
            json_string = json_create(row[1], row[4], row[5], row[6], row[7], row[3], row[9]);
        }
        //initialization above
    
	
	printf("Created: %s\n", json_string);
}    
sending(json_string, send, pid);
    //testing and printing in sending function

    mysql_free_result(result);
    mysql_close(con);

    return 0;
}
