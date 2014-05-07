#include <stdio.h>
#include <mongo.h>
#include <stdlib.h>
#include <json/json.h>

char *fetch_question(char *questionID) {

	mongo conn[1];
	int status = mongo_client( conn, "127.0.0.1", 27017 );

	if( status != MONGO_OK ) {
		switch ( conn->err ) {
			case MONGO_CONN_NO_SOCKET:  printf( "no socket\n" ); return NULL;
			case MONGO_CONN_FAIL:       printf( "connection failed\n" ); return NULL;
			case MONGO_CONN_NOT_MASTER: printf( "not master\n" ); return NULL;
		}
	}

	//temp variables
	bson_type type;
	char *string;

	//setup query bson
	bson query[1];
	bson_init(query);
	bson_append_int(query, "ID", atoi(questionID));
	bson_finish(query);

	//setup field restriction bson
	bson fields[1];
	bson_init(fields);
	/* --restrictions here -- */

	bson_finish(fields);

	//setup cursor and connect to db
	mongo_cursor cursor[1];
	mongo_cursor_init(cursor, conn, "Quiz.Question" );
	mongo_cursor_set_query(cursor, query );

	//iterate through the result and store to doc
	bson *doc;
	while (mongo_cursor_next(cursor) == MONGO_OK) {
		doc = (bson *)mongo_cursor_bson(cursor);
	}

	//prepare json
	struct json_object *root;
	struct json_object *option;

	root = json_object_new_object();
	option = json_object_new_array();

	//iterating the result
	bson_iterator it, st;
	bson_iterator_init(&it, doc);
	while ((type=bson_iterator_next(&it))!=BSON_EOO) {
		switch(type) {
			case 1:
				printf("Question ID: %d\n", bson_iterator_int(&it));
				json_object_object_add(root, "ID", json_object_new_int(bson_iterator_int(&it)));
				break;
			case 2:
				string = (char*)bson_iterator_string(&it);
				if(strcmp(bson_iterator_key(&it), "Question")==0) {
					printf("Question: %s\n", string);
					json_object_object_add(root, "Question", json_object_new_string(bson_iterator_string(&it)));
				}
				else {
					printf("Correct Answer: %s\n", string);
					json_object_object_add(root, "Correct", json_object_new_string(bson_iterator_string(&it)));
				}
				break;
			case 4:
				bson_iterator_subiterator(&it, &st);
				for(int i=0; i<4; i++) {
					bson_iterator_next(&st);
					json_object_array_add(option, json_object_new_string(bson_iterator_string(&st)));
				}
				json_object_object_add(root, "Answer", option);
				//more adaptive parse
				//while(bson_iterator_next(&st)!=BSON_EOO) {
				//	printf("Answer: %s\n", bson_iterator_string(&st));
				//}
				break;
		}
	}

	bson_destroy(query);
	bson_destroy(fields);
	mongo_cursor_destroy(cursor);
	mongo_destroy( conn );

	char *json_string = (char*)malloc(strlen(json_object_get_string(root))+1);
	strcpy(json_string, json_object_get_string(root));

	return json_string;
}

/*
int main()
{
	char *first_question = fetch_question("1");
	char *second_question = fetch_question("2");

	printf("%s %s\n", first_question, second_question);

	return 0;
}
*/
