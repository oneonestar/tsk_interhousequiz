#include <bcon.h>
#include <bson.h>
#include <stdio.h>
#include <mongoc.h>
#include <json-glib/json-glib.h>

#include "include/layout.h"	/* <-ncurses.h */
#include "include/webserver_connector.h"

void log_handler(mongoc_log_level_t log_level, const char *log_domain, const char *message, void *user_data)
{

}

void webserver_init(char *host, char *port)
{
	// initialize mongodb connection parameters
	mongoc_log_set_handler(log_handler, NULL);
	sprintf(address_str, "mongodb://%s:%s/", host, port);
}

void webserver_update_answer(int state)
{
	mongoc_init ();

	// setup client and collection descriptor
	mongoc_client_t *client = mongoc_client_new(address_str);
	sprintf(collection_str, "%s", "display_ctl");
	mongoc_collection_t *collection = mongoc_client_get_collection(client, "meteor", collection_str);

	// control_id 1 refers to control of display of answer
	bson_t *query = BCON_NEW ("control_id", BCON_INT32(1));
	bson_error_t error;
	bson_t *update = NULL;

	// if state is 1, reveal answer
	if(state==1) {
		update = BCON_NEW ("$set", "{", "answer_status", BCON_BOOL(true), "}");
	}
	else if(state==0) {
		update = BCON_NEW("$set", "{", "answer_status", BCON_BOOL(false), "}");
	}

	// issue update instruction
	if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
		wprintw(msg_content, "Web server push failed: %s\n", error.message);
	}

	// cleanup
	bson_destroy(update);
	bson_destroy(query);

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

void webserver_update_question(char *question_id)
{
	// question id consists of catalog value, group and question id
	char catalog[2];
	int group, qid;
	sscanf(question_id, "%c:%d:%d", &catalog[0], &group, &qid);
	catalog[1] = 0;

	// initialize mongodb client descriptor setup
	mongoc_init();
	mongoc_client_t *client = mongoc_client_new(address_str);
	sprintf(collection_str, "%s", "display_ctl");
	mongoc_collection_t *collection = mongoc_client_get_collection(client, "meteor", collection_str);
	bson_error_t error;

	// setup query statement and update statement
	bson_t *query = BCON_NEW ("control_id", BCON_INT32(0));
	bson_t *update = BCON_NEW ("$set", "{", "question_id", "{", "catalog",  BCON_UTF8(catalog), "group", BCON_INT32(group), "qid", BCON_INT32(qid), "}", "}");

	// issue instruction
	if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
		wprintw(msg_content, "Web server push failed: %s\n", error.message);
	}

	// cleanup
	bson_destroy(update);
	bson_destroy(query);

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

void webserver_update_score(char **name, int *new_score, int no_of_participants)
{
	// setup mongodb client descriptor and point to score collection
	mongoc_init ();
	mongoc_client_t *client = mongoc_client_new(address_str);
	sprintf(collection_str, "%s", "scores");
	mongoc_collection_t *collection = mongoc_client_get_collection(client, "meteor", collection_str);
	mongoc_bulk_operation_t *bulk = mongoc_collection_create_bulk_operation (collection, true, NULL);
	bson_error_t error;

	// prepare for mongodb bulk operations
	int i = 0;
	for(i=0; i<no_of_participants; i++) {
		bson_t *query = BCON_NEW ("house", BCON_UTF8(name[i]));
		bson_t *update = BCON_NEW ("$set", "{", "score", BCON_INT32 (new_score[i]), "}");
		mongoc_bulk_operation_update(bulk, query, update, true);
		bson_destroy(update); bson_destroy(query);
	}

	// issue updates in bulk
	if(!mongoc_bulk_operation_execute (bulk, NULL, &error)) {
		wprintw(msg_content, "Web server push failed: %s\n", error.message);
	}

	// cleanup
	mongoc_bulk_operation_destroy (bulk);
	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

char *retrieve_questions() 
{
	// setup client and collection descriptor
	mongoc_client_t *client = mongoc_client_new(address_str);
	mongoc_collection_t *collection = mongoc_client_get_collection(client, "meteor", "questions");
	const bson_t *doc;
	mongoc_cursor_t *cursor;

	// control_id 1 refers to control of display of answer
	bson_t *query = bson_new();

	// init json builder and begin array
	JsonBuilder *builder = json_builder_new();
	json_builder_begin_object(builder);
	json_builder_set_member_name(builder, "Questions");
	json_builder_begin_array(builder);

	// perform query and iterate cursor, add returned json string to array
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	while(mongoc_cursor_next(cursor, &doc)) {
		char *json = bson_as_json(doc, NULL);
		json_builder_add_string_value(builder, json);
		free(json);
	}

	// end array
	json_builder_end_array(builder);
	json_builder_end_object(builder);

	// extract root to get new json generator to return json string
	JsonGenerator *generator = json_generator_new();
	JsonNode *root_node = json_builder_get_root(builder);
	json_generator_set_root(generator, root_node);
	gchar *str = json_generator_to_data(generator, NULL);

	char *json_str = malloc(sizeof(char*)*strlen(str)+1);
	sprintf(json_str, "%s\n", str);
	free(str);

	// cleanup
	bson_destroy (query);
	mongoc_cursor_destroy (cursor);
	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
	json_node_free (root_node);
	g_object_unref (generator);
	g_object_unref (builder);

	return json_str;
}

