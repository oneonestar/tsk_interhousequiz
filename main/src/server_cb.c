#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <search.h>

#include "include/server_cb.h"
#include "include/server.h"
#include "include/link_list.h"
#include "include/non_blocking_socket.h" /* -> event2/util.h + event2/listener.h */
#include "include/layout.h"
#include "include/db_redis.h"
#include "include/utilities.h"
#include "include/score.h"

struct Server_func_cb {
	char name[10];
	void (*func)(char, char*);
};

struct hsearch_data func_table;

void display_question_cb(char x, char *value) {

	char question_json[500];

	//to read a question from database
	printf("Reading Question: %s\n", value);
	
	//procedure to get json string from database module
	db_con *con = db_connect();									//initiate mysql connection
	sprintf(question_json, "question:%s", db_get_result(con, value));	//get question with question ID and store in buffer
	db_close(con);											//close the connection
	
	//send message to webserver to show question
	printf("Sending Question to Web Server: ");
	send_message(webServer, webPort, question_json);
}

void display_answer_cb(char x, char *value_int) {

	printf("Requesting to show answer: ");
	send_message(webServer, webPort, "answer:{}");
}	

//initialize hash tables and store function pointers
void hash_table_init() {

	memset(&func_table, 0, sizeof(struct hsearch_data));
	hcreate_r(100, &func_table);

	ENTRY item;
	ENTRY *ret;

	item.key = "Score+Minus";
	item.data = (void*)malloc(sizeof(struct Server_func_cb));
	((struct Server_func_cb*)(item.data))->func = &minus_score;
	hsearch_r(item, ENTER, &ret, &func_table);

	item.key = "Score+Add";
	item.data = (void*)malloc(sizeof(struct Server_func_cb));
	((struct Server_func_cb*)(item.data))->func = &add_score;
	hsearch_r(item, ENTER, &ret, &func_table);

	item.key = "Score+Update";
	item.data = (void*)malloc(sizeof(struct Server_func_cb));
	((struct Server_func_cb*)(item.data))->func = &update_score;
	hsearch_r(item, ENTER, &ret, &func_table);


/*
	// minus scores
	struct Hashcell *minus_score_hash = (struct Hashcell*)malloc(sizeof(struct Hashcell));
	strcpy(minus_score_hash->id, "Score+Minus");
	minus_score_hash->func = &minus_score;
	HASH_ADD_STR(func_table, id, minus_score_hash);

	// update scores
	struct Hashcell *update_score_hash = (struct Hashcell*)malloc(sizeof(struct Hashcell));
	strcpy(update_score_hash->id, "Score+Update");
	update_score_hash->func = &update_score;
	HASH_ADD_STR(func_table, id, update_score_hash);

	// add scores
	struct Hashcell *add_score_hash = (struct Hashcell*)malloc(sizeof(struct Hashcell));
	strcpy(update_score_hash->id, "Score+Add");
	add_score_hash->func = &add_score;
	HASH_ADD_STR(func_table, id, add_score_hash);


	// display questions
	struct Hashcell *question_push_hash = (struct Hashcell*)malloc(sizeof(struct Hashcell));
	strcpy(question_push_hash->id, "Question+Next");
	question_push_hash->func = &display_question_cb;
	HASH_ADD_STR(func_table, id, question_push_hash);

	// show answer
	struct Hashcell *answer_push_hash = (struct Hashcell*)malloc(sizeof(struct Hashcell));
	strcpy(answer_push_hash->id, "Answer+");
	answer_push_hash->func = &display_answer_cb;
	HASH_ADD_STR(func_table, id, answer_push_hash);
*/
}


//server main loop and call back function to parse instruction from telnet
void on_read_cb(struct bufferevent *bev, void *ctx)
{
	//read from socket buffer
	//evbuffer created to get input from buffer, obtain lenth of data in buffer with evbuffer_get_length()
	//allot memory and call evbuffer_remove() to copy data into recvBuffer and drain data inside buffer
	char *recvBuff;
	struct evbuffer *input = bufferevent_get_input(bev);	//read the buffer
	struct Info *inf = ctx;									//get information about the connection
	size_t len = evbuffer_get_length(input);				//get length
	if(len) {
		printf("\nData of length %zu received from %s:%s\n", len, inf->address, inf->port);
		recvBuff = (char*)malloc(sizeof(char)*(len+1));
		if(evbuffer_remove(input, recvBuff, len)<0) {
			recvBuff[len] = 0;
			printf("DEBUG: on_read_cb(): copy form evbuffer failed!\n");
		}
		else {
			recvBuff[len] = 0;
		}
		
	}

	//parsing variables
	//instruction=command catag, option=action to be taken; value=a char value; data=an int value
	char instruction[10], option[10], value[100], data;	//for sscanf
	int intInstruction, intOption;				//for parsing
	//struct Hashcell *hashed_func;

	//a buffer for storing returned string from functions
	char buffer[5000];
	
	//DEBUG
	printf("Command Received: %s\n", recvBuff);

	//process instruction
	sscanf(recvBuff, "%s %s %c %s", instruction, option, &data, value);
	intInstruction = parse_instruction(instruction);			//read instructions
	intOption = parse_option(intInstruction, option);			//read options

	//free recvBuff after use
	free(recvBuff);
	
	//error check
	if(intInstruction==-1||intOption==-1) {
		printf("invalid instruction!\n");
	}

	//perform query on hash table
	sprintf(buffer, "%s+%s", instruction, option);
	ENTRY entry;
	entry.key = buffer;
	ENTRY *found;
	hsearch_r(entry, FIND, &found, &func_table);
	//found->data->func(data, value);
	printf("id is: %s\n", ((struct Server_func_cb*)(found->data))->name);
	void (*func)(char, char*) = ((struct Server_func_cb*)(found->data))->func;
	func(data, value);

	//push score to webserver
	//save_score("score_backup.dat");
	push_score(webServer, webPort);

	
/*
	//parse instructions and options
	switch(intInstruction) {

		case 0:
			//end
			printf("Exiting...\n");
			exit(0);
			break;
		case 1:
			//database process
			switch(intOption) {
				case 1:
					//to read a question from database
					printf("Reading Question: %s\n", value);
					
					//procedure to get json string from database module
					db_con *con = db_connect();									//initiate mysql connection
					sprintf(buffer, "question:%s", db_get_result(con, value));	//get question with question ID and store in buffer
					db_close(con);											//close the connection
					
					//send message to webserver to show question
					printf("Sending Question to Web Server: ");
					send_message(webServer, webPort, buffer);
					break;
			}
			break;
		case 2:
			//score process
			switch(intOption) {
				case 1:
					//call change score
					add_score(char_to_house(data), atoi(value));
					break;
				case 2:
					//call update to overwrite
					update_score(char_to_house(data), atoi(value));
					break;
				case 3:
					//call change score to minus scores
					minus_score(char_to_house(data), atoi(value));
					break;
			}

			//push score to webserver
			save_score("score_backup.dat");
			push_score(webServer, webPort);
			break;

		case 3:
			//Buzzer
			switch(intOption) {
				//being able to enter this block implies that the 'option' variable is safe to use
				case 0:
					printf("Requesting to reset Buzzer\n");
					send_message(buzzerServer, buzzerPort, option);
					break;

				case 1:
					printf("Requesting to enable Buzzer\n");
					send_message(buzzerServer, buzzerPort, option);
					break;

				case 2:
					printf("Requesting to disable Buzzer\n");
					send_message(buzzerServer, buzzerPort, option);
					break;
			}
			
			break;

		case 4:
			//answer
			printf("Requesting to show answer: ");
			send_message(webServer, webPort, "answer:{}");
			break;

		case 5:
			switch(intOption) {
			
				case 1:
					printf("Requesting to show the score panel: ");
					send_message(webServer, webPort, "ui:{\"score\":\"show\"}");
					break;
			
				case 2:
					printf("Requesting to hide the score panel: ");
					send_message(webServer, webPort, "ui:{\"score\":\"hide\"}");
					break;
			}

			break;
	}
*/	
	//broadcast feedback and ack
	listBroadcast(theList, "ACK from server");

	//clear information
	memset(&recvBuff, 0, sizeof(recvBuff));
	memset(&buffer, 0, sizeof(buffer));
	intInstruction = 0;
	intOption = 0;
}

int parse_instruction(char *instruction)
{
	if(strcmp(instruction, "Question")==0) {
		return 1;
	}
	else if(strcmp(instruction, "Score")==0) {
		return 2;
	}
	else if(strcmp(instruction, "Buzzer")==0) {
		return 3;
	}
	else if(strcmp(instruction, "Answer")==0) {
		return 4;
	}
	else if(strcmp(instruction, "UI")==0) {
		return 5;
	}
	else if(strcmp(instruction, "Quit")==0) {
		return 0;
	}
	return -1;
}

int parse_option(int instruction, char *option)
{
	switch(instruction) {

		case 1:
			if(strcmp(option, "Next")==0) {
				return 1;
			}
			break;

		case 2:
			if(strcmp(option, "Add")==0) {
				return 1;
			}
			else if(strcmp(option, "Update")==0) {
				return 2;
			}
			else if(strcmp(option, "Minus")==0) {
				return 3;
			}
			break;

		case 3:
			if(strcmp(option, "0")==0) {
				return 0;
			}
			else if(strcmp(option, "1")==0) {
				return 1;
			}
			else if(strcmp(option, "2")==0) {
				return 2;
			}
			break;

		case 4:
			return 0;
			break;

		case 5:
			if(strcmp(option, "Show")==0) {
				return 1;
			}
			else if(strcmp(option, "Hide")==0) {
				return 2;
			}
			break;

		case 0:
			return 0;
			break;
	}
	return -1;
}
