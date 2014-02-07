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
#include <wchar.h>
#include <locale.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>

const int socket_port = 8889;
char socket_addr[] = "127.0.0.1";

const char machine_name[] = "localhost";
const char sql_user[] = "test";
const char sql_pwd[] = "testing";
const char sql_table_name[] = "test";


char* string_create(char *id, char *question, char* A, char* B, char *C, char *D, char *correct, char *path)
{	
         //create a string with memory allocation
         char* myStr;
         myStr = (char*) malloc (sizeof(char)*6000);
 
         //concatenate all objects required
         sprintf(myStr, "{\"ID\":\"%s\",\"Question\":\"%s\",\"Options\":[\"%s\",\"%s\",\"%s\",\"%s\"],\"Answer\":\"%s\",\"Path\":\"%s\"}",id, question, A, B, C, D, correct, path);
	 printf("STRING_CREATE: %s\n", myStr);
         return myStr; 
}
   
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

void send_id(int fd[2], char* cid)
{
    //char cid[] = "2";
    close(fd[0]);
    printf("Parent: I have sent the id: %s\n", cid);
    write(fd[1], cid, sizeof(cid));
    close(fd[1]);
}

char* get_string(int fd[2])
{
    /* write through the pipe */
    char* MyStr;
    MyStr = (char*) malloc (6000);

    close(fd[1]);
    read(fd[0], MyStr, 600);
    close(fd[0]);

    return MyStr;
}

void send_string(char* json_string, int fd[2])
{
    close(fd[0]);

    printf("Child: The string is: %s\n", json_string);
    //printf("Child: The string is: %d\n", wcslen(json_string));
    write(fd[1], json_string, strlen(json_string)+1);
    close(fd[1]);
}

MYSQL* sql_connect()
{
    MYSQL* con = mysql_init(NULL);
    mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8");
    if (con == NULL) {
        printf("%s", "The connection failed, Please try again.\n");
        exit(-1);
    }

    //(localhost, name of user, password, database)
    if (mysql_real_connect(con, machine_name, sql_user, sql_pwd, sql_table_name, 0, NULL, 0) == NULL) {
        printf("%s", "The connection failed, Please enter the correct password again\n");
        mysql_close(con);
        exit(-1);
    }
    return con;
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

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(socket_port);
    inet_pton(AF_INET, address, &serv_addr.sin_addr);

    connect(point, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if((trap = write(point, send, strlen(send)))<0) {
        printf("Socket: connection error\n");
    }
}

char* sql_get_result(MYSQL* con, char* cid)
//int main()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[100] = "Select * from data where id = ";
    int track[40];
    init_track(track);
    char* pushstring;
    char pullstring[600];

    //The program starts with successful connection
    //items retreiving

        strcat (query, cid);//concatenation of the id and the query

        result = get_result(con, query);
	
        while((row = mysql_fetch_row(result))) {

	pushstring = string_create(cid, row[1], row[4], row[5], row[6], row[7], row[3], row[9]);

        }
        //initialization above

        printf("Created: %s\n", pushstring);
        //testing and printing in sending function
	
	//asking the result from the child and copy to the pullstring
	//char sql_fetched []="Question:";
	//sprintf(sql_fetched, pushstring);
	
	//printf("Parent: %s\n", sql_fetched);
        //pushresult(socket_addr, sql_fetched);
    
	mysql_free_result(result);
	return pushstring;
}

/*
int main()
{
    MYSQL *con = sql_connect();
    char* result;
    result = sql_get_result(con, "2");
    printf("Main: %s\n", result);
    mysql_close(con);
    return 0;
}
*/
