#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/layout.h"
#include "include/json_msg.h"	/* ->json_glib/json_glib.h; ->json_glib/json_gobject.h */

/* decode json msg from control panels */
unsigned decode_json(char *json, char **instruction, char **action, char **team, char **value)
{
	GError *error = NULL;

	/* new parser */
	JsonParser *parser = json_parser_new();
	if(json_parser_load_from_data(parser, json, -1, &error)==0) {
		wprintw(msg_content, "%s\n", error->message);
		wrefresh(msg_content);
		g_error_free(error);
		return 1;
	}

	/* new reader to read streams*/
	JsonReader *reader = json_reader_new(json_parser_get_root(parser));

	/* parse individual elements */
	if(json_reader_read_member(reader, "Instruction")) {
		const gchar *str = json_reader_get_string_value(reader);
		*instruction = malloc(sizeof(char)*strlen(str)+1);
		strcpy(*instruction, str);
	}
	else {
		*instruction = malloc(1);
		strcpy(*instruction, "");
	}
	json_reader_end_member(reader);

	if(json_reader_read_member(reader, "Action")) {
		const gchar *str = json_reader_get_string_value(reader);
		*action = malloc(sizeof(char)*strlen(str)+1);
		strcpy(*action, str);
	}
	else {
		*action = malloc(1);
		strcpy(*action, "");
	}
	json_reader_end_member(reader);

	if(json_reader_read_member(reader, "Team")) {
		const gchar *str = json_reader_get_string_value(reader);
		*team = malloc(sizeof(char)*strlen(str)+1);
		strcpy(*team, str);
	}
	else {
		*team = malloc(1);
		strcpy(*team, "");
	}
	json_reader_end_member(reader);

	if(json_reader_read_member(reader, "Value")) {
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

	/* return okay */
	return 0;
}

/* return an encoded string of json msg intended as ACK to control panel */
gchar *encode_json(char *instruction, char *action, char *team, char *value, int status)
{
	/* new json builder */
	JsonBuilder *builder = json_builder_new();
	json_builder_begin_object (builder);

	/* push elements into object */
	json_builder_set_member_name(builder, "Data");
	json_builder_begin_object(builder);

	json_builder_set_member_name(builder, "Instruction");
	if(instruction[0]=='\0' || instruction==NULL) {
		json_builder_add_null_value(builder);
	}
	else {
		json_builder_add_string_value(builder, instruction);
	}

	json_builder_set_member_name(builder, "Action");
	if(action[0]=='\0' || action==NULL) {
		json_builder_add_null_value(builder);
	}
	else {
		json_builder_add_string_value(builder, action);
	}

	json_builder_set_member_name(builder, "Team");
	if(team[0]=='\0' || action==NULL) {
		json_builder_add_null_value(builder);
	}
	else {
		json_builder_add_string_value(builder, team);
	}

	json_builder_set_member_name(builder, "Value");
	if(value[0]=='\0' || value==NULL) {
		json_builder_add_null_value(builder);
	}
	else {
		json_builder_add_string_value(builder, value);
	}
	
	json_builder_end_object(builder);

	json_builder_set_member_name(builder, "Response");
	json_builder_add_boolean_value(builder, status);

	json_builder_end_object (builder);

	/* generate json by adding root built by builder to generator */
	JsonGenerator *generator = json_generator_new();
	JsonNode *root = json_builder_get_root(builder);
	json_generator_set_root(generator, root);
	json_generator_set_pretty(generator, 0);	/* set message not to be pretty to prevent any return char because client determine end of msg by return char */

	/* retrieve json string with memory alloted for return, free with g_free() after use*/
	gchar *json = json_generator_to_data (generator, NULL);

	/* copy string to heap memory and append null char. */
	char *json_string = (char*)malloc(strlen(json)+1);
	sprintf(json_string, "%s\n", json);

	/* free resources */
	json_node_free(root);
	g_object_unref(builder);
	g_object_unref(generator);
	g_free(json);

	return json_string;
}
