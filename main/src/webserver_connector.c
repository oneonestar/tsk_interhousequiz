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
	sprintf(collection_str, "%s", "scores");
}

void webserver_update_score(char *name, int new_score)
{
	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_t *doc = NULL;
	bson_t *update = NULL;
	bson_t *query = NULL;

	mongoc_init ();

	client = mongoc_client_new(address_str);
	collection = mongoc_client_get_collection(client, "meteor", collection_str);

	query = BCON_NEW ("house", BCON_UTF8(name));
	update = BCON_NEW ("$set", "{",
		"score", BCON_INT32 (new_score),
		"}");

	if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
		char *bson_str = bson_as_json(doc, NULL);
		wprintw(msg_content, "Web server push failed: %s, %s\n", error.message,bson_str);
		bson_destroy(doc);
		free(bson_str);
	}

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

/*
int main(int argc, char *argv[]) {

	memset(&address_str, 0, sizeof(address_str));
	sprintf(address_str, "mongodb://%s:%s/", argv[1], argv[2]);
	sprintf(collection_str, "scores");
	printf("%s\n", address_str);
	printf("%s: %d\n", argv[3], atoi(argv[4]));

	update_webserver_score(argv[3], atoi(argv[4]));
	return 0;
}
*/
