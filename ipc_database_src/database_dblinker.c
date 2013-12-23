#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include <string.h>

using namespace std;

struct q
{
	char question[50];
	char answer[200];
	char oa[50];
	char ob[50];
	char oc[50];
	char od[50];

};

int main()
{
	//string question, answer, oa, ob, oc, od;
	int id;

	MYSQL *con = mysql_init(NULL);

	if (con == NULL)
	{
		printf("%s", "The connection failed, Please try again.\n");
	}
	printf("%s", "starts");
	if (mysql_real_connect(con, "localhost", "root", "123456", "data", 0, NULL, 0) == NULL)
	{
		printf("%s", "The connection failed, Please reenter the correct password\n");
		mysql_close(con);
	}
	printf("%s", "The connection succeeded\n");	
//The program starts with successful connection
	//items retreiving
	id = 1;
	char tid=(char)id;
	char query [100];
	sprintf (query,  "SELECT * from Table where id='", tid, "'");
	
	if (mysql_query(con, query))
	{
		exit(-1);
	}

	MYSQL_RES *result = mysql_store_result(con);

	if (result)
	{
		exit(-1);
	}

	MYSQL_ROW row;

	if(row = mysql_fetch_row(result))
	{
		printf("%s", "fetching process failed");
		exit(-1);
	}
	
	q target;
	
	strcpy(target.question, "");
	strcpy(target.answer, "");
	strcpy(target.oa, "");
	strcpy(target.ob, "");
	strcpy(target.oc, "");
	strcpy(target.od, "");
	//initialization above

	strcpy(target.question, (char*)row[1]);
	strcpy(target.answer, (char*)row[3]);
	if ((string)row[2]== "MC")
	{
 		strcpy(target.oa, (char*)row[4]);
		strcpy(target.ob, (char*)row[5]);
		strcpy(target.oc, (char*)row[6]);
		strcpy(target.od, (char*)row[7]);
	}
	mysql_free_result(result);
	mysql_close(con);

	return 0;
}
