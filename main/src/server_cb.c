#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <glib.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "include/server_cb.h"
#include "include/server.h"
#include "include/link_list.h"
#include "include/non_blocking_socket.h" /* -> event2/util.h + event2/listener.h */
#include "include/layout.h"
#include "include/utilities.h"
#include "include/score_db.h"
#include "include/question_db.h"

/* migrating to msg in json */
#include "include/json_msg.h"

/* function table for server event callback */
GHashTable *func_table;

/* Print Ctrl character. eg. convert the \n into "0xA"... */
void buffrepr(const char * buff, int size, char * result, int resultSize)
{
	while (size && resultSize) {

		int print_count; 

		if(!isalpha(*buff))
			print_count = snprintf(result, resultSize, "0x%X", *buff);
		else
			print_count = snprintf(result, resultSize, "%c", *buff);

		resultSize -= print_count;
		result += print_count;
		--size;
		++buff;

		if (size && resultSize) {
			int print_count = snprintf(result, resultSize, " ");
			resultSize -= print_count;
			result += print_count;
		}
	}
}

void display_question_cb(char x, char *value)
{
	/* to read a question from database */
	char question_msg[1000];
	wprintw(msg_content, "Reading Question: %s\n", value);
	char *question_json = get_db_question(value);

	char *question = NULL, *A = NULL, *B = NULL, *C = NULL, *D = NULL, *path = NULL, *correct = NULL;
	decode_question(question_json, &question, &A, &B, &C, &D, &path, &correct);
	webserver_update_question(question, A, B, C, D, path, correct);
	free(question); free(A); free(B); free(C); free(D); free(path); free(correct);

	sprintf(question_msg, "question:%s\n", question_json);	/* get question with question ID and store in buffer */
	wprintw(msg_content, "%s\n", question_json);

	/* send message to webserver to show question */
	wprintw(msg_content, "Sending Question to Web Server: \n");
	wrefresh(msg_content);
	send_message(webServer, webPort, question_msg);
	free(question_json);
}

void display_answer_cb(char x, char *value_int)
{
	wprintw(msg_content, "Requesting to show answer: \n");
	wrefresh(msg_content);
	//send_message(webServer, webPort, "answer:{}");
	webserver_update_answer(1);
}

void buzzer_reset_cb(char x, char *value)
{
	send_message(buzzerServer, buzzerPort, "0");
}

void buzzer_enable_cb(char x, char *value)
{
	send_message(buzzerServer, buzzerPort, "1");
}

void buzzer_disable_cb(char x, char *value)
{
	send_message(buzzerServer, buzzerPort, "w");
}

void ui_show_score_cb(char x, char *value)
{
	send_message(webServer, webPort, "ui:{\"score\":\"show\"}");
}

void ui_hide_score_cb(char x, char *value)
{
	send_message(webServer, webPort, "ui:{\"score\":\"hide\"}");
}

/* initialize hash tables and store function pointers */
void hash_table_init()
{
	func_table = g_hash_table_new(g_str_hash, g_str_equal);

	/* score database operation */
	g_hash_table_insert(func_table, "Score+Answer", score_db_set);

	/* question answer operation */
	g_hash_table_insert(func_table, "Question+Next", display_question_cb);
	g_hash_table_insert(func_table, "Answer+", display_answer_cb);

	/* buzzer operation */
	g_hash_table_insert(func_table, "Buzzer+0", buzzer_reset_cb);
	g_hash_table_insert(func_table, "Buzzer+1", buzzer_enable_cb);
	g_hash_table_insert(func_table, "Buzzer+2", buzzer_disable_cb);

	/* UI operation */
	g_hash_table_insert(func_table, "UI+Show", ui_show_score_cb);
	g_hash_table_insert(func_table, "UI+Hide", ui_hide_score_cb);
}

/* server main loop and call back function to parse instruction from telnet */
void on_read_cb(struct bufferevent *bev, void *ctx)
{
	/*
	 * read from socket buffer
	 * evbuffer created to get input from buffer, obtain lenth of data in buffer with evbuffer_get_length()
	 * allot memory and call evbuffer_remove() to copy data into recvBuffer and drain data inside buffer
	 */
	char *recvBuff;
	struct evbuffer *input = bufferevent_get_input(bev);	/* read the buffer */
	struct Info *inf = ctx;					/* get information about the connection */
	size_t len = evbuffer_get_length(input);		/* get length */
	if(len) {
		wprintw(msg_content,"\nData of length %zu received from %s:%d\n", len, get_address_string(inf), get_port_int(inf));
		wrefresh(msg_content);
		recvBuff = (char*)malloc(sizeof(char)*(len+1));

		if(evbuffer_remove(input, recvBuff, len) < 0) {
			recvBuff[len] = 0;
			wprintw(msg_content, "DEBUG: on_read_cb(): copy form evbuffer failed!\n");
			wrefresh(msg_content);
		} else {
			recvBuff[len] = 0;
		}

	}

	/* parsing variables */
	/* instruction=command catag, option=action to be taken; value=a char value; data=an int value */
	char instruction[10], option[10], value[100], data[2];	/* for sscanf */
	memset(instruction, 0, sizeof(instruction));
	memset(option, 0, sizeof(option));
	memset(value, 0, sizeof(value));
	memset(data, 0, sizeof(data));

	/* a buffer for storing returned string from functions */
	char buffer[5000];

	/* DEBUG */
	char printBuffer[5000];

	buffrepr(recvBuff, len, printBuffer, sizeof(printBuffer));

	wprintw(msg_content, "Command Received: %s\n", printBuffer);
	wrefresh(msg_content);

	/* process instruction */
	sscanf(recvBuff, "%s %s %s %s", instruction, option, data, value);

	/* free recvBuff after use */
	free(recvBuff);

	/* perform query on hash table */
	sprintf(buffer, "%s+%s", instruction, option);
	wprintw(msg_content, "search: %s\n", buffer);
	wrefresh(msg_content);

	gpointer *found;
	if((found=g_hash_table_lookup(func_table, buffer)) != NULL) {
		void (*func)(char, char*) = (void*)found;
		func(data[0], value);

		/* send latest score to web server */
		score_publish();

		/* broadcast feedback and ack, free returned json string after use */
		char *str = encode_json(instruction, option, data, value, 1);
		wprintw(msg_content, "%s", str);
		wrefresh(msg_content);
		listBroadcast(theList, str);
		free(str);
	} else {
		/* show that request is invalid */
		char *str = encode_json(instruction, option, data, value, 0);
		wprintw(msg_content, "request %s not found in hash table!\n", buffer);
		wprintw(msg_content, "%s", str);
		wrefresh(msg_content);
		listBroadcast(theList, str);
		free(str);
	}

	/* clear information */
	memset(&recvBuff, 0, sizeof(recvBuff));
	memset(&buffer, 0, sizeof(buffer));
}
