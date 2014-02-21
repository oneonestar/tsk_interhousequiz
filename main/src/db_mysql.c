#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdbool.h>
#include <my_global.h>
#include "db_mysql.h"

static const char machine_name[] = "localhost";
static const char sql_user[] = "test";
static const char sql_pwd[] = "testing";
static const char sql_table_name[] = "test";

/**
 * @brief Convert SQL fields into a JSON string
 * @param id question id
 * @param question the question part
 * @param A multiple choice value A
 * @param B multiple choice value B
 * @param C multiple choice value C
 * @param D multiple choice value D
 * @param correct the correct answer ("A","B","C"or"D")
 * @param path the url of the image (only if a image question)
 * @return Allocate a new string contains the JSON question.
 * @warning Caller should free the string using free().
 */
static char* string_create( char *id, char *question, char* A, char* B, char *C, char *D, char *correct, char *path );

static char* string_create( char *id, char *question, char* A, char* B, char *C, char *D, char *correct, char *path )
{
    //create a string with memory allocation
    char* myStr;
    myStr = ( char* ) malloc ( sizeof( char )*6000 );

    //concatenate all objects required
    sprintf( myStr, "{\"ID\":\"%s\",\"Question\":\"%s\",\"Options\":[\"%s\",\"%s\",\"%s\",\"%s\"],\"Answer\":\"%s\",\"Path\":\"%s\"}",
             id, question, A, B, C, D, correct, path );
    printf( "STRING_CREATE: %s\n", myStr );
    return myStr;
}

MYSQL* db_connect()
{
    MYSQL* con = mysql_init( NULL );
    mysql_options( con, MYSQL_SET_CHARSET_NAME, "utf8" );
    if ( con == NULL )
    {
        printf( "%s", "The connection failed, Please try again.\n" );
        exit( -1 );
    }

    //(localhost, name of user, password, database)
    if ( mysql_real_connect( con, machine_name, sql_user, sql_pwd, sql_table_name, 0, NULL, 0 ) == NULL )
    {
        printf( "%s", "The connection failed, Please enter the correct password again\n" );
        mysql_close( con );
        exit( -1 );
    }
    return con;
}

MYSQL_RES* get_result( MYSQL* con, char* query )
{
    MYSQL_RES* result;
    mysql_query( con, query );

    result = mysql_store_result( con );
    if ( !result )
    {
        printf( "%s\n", "Result" );
        exit( -1 );
    }
    return result;
}

char* db_get_result( MYSQL* con, char* cid )
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[100] = "Select * from data where id = ";
    char* pushstring;

    //The program starts with successful connection
    //items retreiving

    strcat ( query, cid ); //concatenation of the id and the query

    result = get_result( con, query );

    while( ( row = mysql_fetch_row( result ) ) )
    {

        pushstring = string_create( cid, row[1], row[4], row[5], row[6], row[7], row[3], row[9] );

    }
    //initialization above

    printf( "Created: %s\n", pushstring );
    //testing and printing in sending function

    mysql_free_result( result );
    return pushstring;
}
void db_close( db_con* con )
{
    mysql_close( con );
}
