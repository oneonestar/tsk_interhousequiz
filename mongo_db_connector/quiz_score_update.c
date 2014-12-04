#include <bcon.h>
#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

char address_str[100];
char collection_str[50];

void update_score(char *name, int new_score) {

	mongoc_collection_t *collection;
	mongoc_client_t *client;
	bson_error_t error;
	bson_oid_t oid;
	bson_t *doc = NULL;
	bson_t *update = NULL;
	bson_t *query = NULL;

	mongoc_init ();

	printf("%s: %d\n", name, new_score);

	client = mongoc_client_new(address_str);
	collection = mongoc_client_get_collection(client, "meteor", collection_str);

	query = BCON_NEW ("house", BCON_UTF8(name));
	update = BCON_NEW ("$set", "{",
		"score", BCON_INT32 (new_score),
		"}");

	if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
		printf ("%s\n", error.message);
		goto fail;
	}

	fail:
	if (doc)
		bson_destroy (doc);
	if (query)
		bson_destroy (query);
	if (update)
		bson_destroy (update);

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
}

int main(int argc, char *argv[]) {

	memset(&address_str, 0, sizeof(address_str));
	sprintf(address_str, "mongodb://%s:%s/", argv[1], argv[2]);
	sprintf(collection_str, "scores");
	printf("%s\n", address_str);
	printf("%s: %d\n", argv[3], atoi(argv[4]));

	update_score(argv[3], atoi(argv[4]));
	return 0;
}
