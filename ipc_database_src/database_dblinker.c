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

wchar_t* string_create(int id, wchar_t *question, wchar_t* A, wchar_t* B, wchar_t *C, wchar_t *D, wchar_t *correct, wchar_t *path)
{	setlocale(LC_ALL, "");
         //create a string with memory allocation
         wchar_t* myStr;
         myStr = (wchar_t*) malloc (6000);
 
         //concatenate all objects required
         swprintf(myStr, 600, L"{'%d','%s','%s','%s','%s','%s','%s','%s'}", id, question, A, B, C, D, correct, path);
	wprintf(L"STRING_CREATE: %ls\n", myStr);
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
            wprintf(L"%s\n", L"The item is already in the track");
            break;
        }
        if(track[index] == 0) {
            wprintf(L"%s\n", L"The track has been updated");
            track[index] = item;
            return;
        }
        index++;
    }
    wprintf(L"%s\n", L"The track is already full");
}

void get_id(char id[3], int fd[2])
{
    close (fd[1]);
    read(fd[0], id, 3);
    wprintf(L"Child: I have receieved the id: %s\n", id);
    close(fd[0]);
}

void send_id(int fd[2])
{
    char cid[] = "2";
    close(fd[0]);
    wprintf(L"Parent: I have sent the id: %s\n", cid);
    write(fd[1], cid, sizeof(cid));
    close(fd[1]);
}

wchar_t* get_string(int fd[2])
{
    /* write through the pipe */
    wchar_t* MyStr;
    MyStr = (wchar_t*) malloc (6000);

    close(fd[1]);
    read(fd[0], MyStr, 600);
    close(fd[0]);

    setlocale(LC_ALL, "");
return MyStr;
}

void send_string(wchar_t* json_string, int fd[2])
{
    close(fd[0]);

    setlocale(LC_ALL, "");
    wprintf(L"Child: The string is: %ls\n", json_string);
    wprintf(L"Child: The string is: %d\n", wcslen(json_string));
    write(fd[1], json_string, wcslen(json_string)*8+1);
    close(fd[1]);
}

void init_con(MYSQL *con)
{
    if (con == NULL) {
        wprintf(L"%s", L"The connection failed, Please try again.\n");
        exit(-1);
    }

    //(localhost, name of user, password, database)
    if (mysql_real_connect(con, "localhost", "testing", "testing", "test", 0, NULL, 0) == NULL) {
        wprintf(L"%s", L"The connection failed, Please enter the correct password again\n");
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
        wprintf(L"%s\n", L"Result");
        exit(-1);
    }
    return result;
}

void pushresult(char address[], wchar_t sendBuff[])
{

    int point;
    int trap;
    struct sockaddr_in serv_addr;

    wchar_t send[600] = L"question:";
	
    strcat(send, sendBuff);
    point = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8889);
    inet_pton(AF_INET, address, &serv_addr.sin_addr);

    connect(point, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if((trap = write(point, send, strlen(send)))<0) {
        wprintf(L"error\n");
    }
}

int main(void)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[100] = "Select * from data where id = ";
    char id[3];
    int track[40];
    init_track(track);
    int intid;
    wchar_t* pushstring;
    wchar_t pullstring[600];

    int send[2];//pipe for sending and receiveng result(json_string)
    int get[2];//pipe for sending and receiving id
    pipe(send);
    pipe(get);
    int pid = fork();

    MYSQL *con = mysql_init(NULL);
    init_con(con);
    wprintf(L"%s", L"The connection succeeded\n");

    //The program starts with successful connection
    //items retreiving

   setlocale(LC_ALL, "");
    if(pid != 0) {
        get_id(id, get);
        intid = atoi(id);
        update_track(track, intid);

        strcat (query, id);//concatenation of the id and the query

        result = get_result(con, query);

        while((row = mysql_fetch_row(result))) {

	wprintf(L"Question: %ls\n", L"和");
	    wchar_t me[600]= L"我是人\n";
    wchar_t y[600];
    wcscpy(y,me);
    setlocale(LC_ALL, "");
    wprintf(L"%ls\n", y);
    wprintf(L"%ls\n", me);



	pushstring = string_create(intid, row[2], row[5], row[6], row[7], row[8], row[3], row[10]);

        }
        //initialization above

        wprintf(L"Created: %ls\n", pushstring);
        send_string(pushstring, send);
        //testing and printing in sending function
    } else {
        send_id(get);
	//asking the result from the child and copy to the pullstring
	swprintf(pullstring,600, L"Question:%ls\n", get_string(send));
        //not yet done, how to deal with the concatenation of the string
	wprintf(L"Parent: %ls\n", pullstring);
        //pushresult("192.168.0.102", pullstring);
    }
    mysql_free_result(result); 
    mysql_close(con);
}

