#include <bcon.h>
#include <bson.h>
#include <stdio.h>
#include <mongoc.h>
#include "include/layout.h"	/* <-ncurses.h */
#include "include/webserver_connector.h"

void log_handler(mongoc_log_level_t log_level, const char *log_domain, const char *message, void *user_data)
{

}

void webserver_init(char *host, char *port)
{
	mongoc_log_set_handler(log_handler, NULL);
	sprintf(address_str, "mongodb://%s:%s/", host, port);
}

void webserver_update_answer(int state)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_t *doc = NULL;
	bson_t *update = NULL;
	bson_t *query = NULL;

	sprintf(collection_str, "%s", "answers");
	mongoc_init ();

	client = mongoc_client_new(address_str);
	collection = mongoc_client_get_collection(client, "meteor", collection_str);

	query = BCON_NEW ("Id", BCON_INT32(1));
	if(state==1) {
		update = BCON_NEW ("$set", "{", "state", BCON_UTF8(""), "}");
	}
	else if(state==0) {
		update = BCON_NEW("$set", "{", "state", BCON_UTF8("hidden"), "}");
	}

	if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
		char *bson_str = bson_as_json(doc, NULL);
		wprintw(msg_content, "Web server push failed: %s, %s\n", error.message,bson_str);
		bson_destroy(doc);
		free(bson_str);
	}
	bson_destroy(update);
	bson_destroy(query);

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

void webserver_update_question(char *question, char *A, char *B, char *C, char *D, char *path, char *correct)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_t *doc = NULL;
	bson_t *update = NULL;
	bson_t *query = NULL;

	sprintf(collection_str, "%s", "questions");
	mongoc_init ();

	client = mongoc_client_new(address_str);
	collection = mongoc_client_get_collection(client, "meteor", collection_str);

	query = BCON_NEW ("Id", BCON_INT32(1));
	update = BCON_NEW ("$set", "{", "content", BCON_UTF8(question), "path", BCON_UTF8(path), "}");

	if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
		char *bson_str = bson_as_json(doc, NULL);
		wprintw(msg_content, "Web server push failed: %s, %s\n", error.message,bson_str);
		bson_destroy(doc);
		free(bson_str);
	}
	bson_destroy(update);
	bson_destroy(query);

	sprintf(collection_str, "%s", "answers");
	collection = mongoc_client_get_collection(client, "meteor", collection_str);
	query = BCON_NEW ("Id", BCON_INT32(1));
	update = BCON_NEW ("$set", "{", "optionA", BCON_UTF8(A), "optionB", BCON_UTF8(B), "optionC", BCON_UTF8(C), "optionD", BCON_UTF8(D), "correct", BCON_UTF8(correct), "state", BCON_UTF8("hidden"), "}");

	if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
		char *bson_str = bson_as_json(doc, NULL);
		wprintw(msg_content, "Web server push failed: %s, %s\n", error.message,bson_str);
		bson_destroy(doc);
		free(bson_str);
	}

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

void webserver_update_score(char **name, int *new_score, int no_of_participants)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;

	bson_error_t error;
	bson_t *update = NULL;
	bson_t *query = NULL;
	mongoc_bulk_operation_t *bulk;

	int i = 0;

	sprintf(collection_str, "%s", "scores");

	mongoc_init ();
	client = mongoc_client_new(address_str);
	collection = mongoc_client_get_collection(client, "meteor", collection_str);
	bulk = mongoc_collection_create_bulk_operation (collection, true, NULL);

	for(i=0; i<no_of_participants; i++) {
		query = BCON_NEW ("house", BCON_UTF8(name[i]));
		update = BCON_NEW ("$set", "{", "score", BCON_INT32 (new_score[i]), "}");
		mongoc_bulk_operation_update(bulk, query, update, true);
	}

	if(!mongoc_bulk_operation_execute (bulk, NULL, &error)) {
		wprintw(msg_content, "Web server push failed: %s\n", error.message);
		bson_destroy(query);
		bson_destroy(update);
		mongoc_collection_destroy (collection);
		mongoc_client_destroy (client);
	}

	mongoc_bulk_operation_destroy (bulk);
	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

