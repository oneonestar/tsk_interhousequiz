#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_msg.h"	/* ->json_glib/json_glib.h; ->json_glib/json_gobject.h */

/* decode json msg from control panels */
void decode_json(char *json, int *option, char **value)
{
	/* new parser */
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, json, -1, NULL);

	/* new reader to read streams*/
	JsonReader *reader = json_reader_new(json_parser_get_root(parser));

	/* parse individual elements */
	if(json_reader_read_member(reader, "option")) {
		gint64 str = json_reader_get_int_value(reader);
		*option = (int)str;

	}
	else {
		*option = -1;
	}
	json_reader_end_member(reader);

	if(json_reader_read_member(reader, "value")) {
		const gchar *str = json_reader_get_string_value(reader);
		*value = malloc(sizeof(char)*strlen(str)+1);
		strcpy(*value, str);
	}
	else {
		*value = malloc(1);
		strcpy(*value, "");
	}
	json_reader_end_member(reader);

	/* free resources */
	g_object_unref(reader);
	g_object_unref(parser);
}
