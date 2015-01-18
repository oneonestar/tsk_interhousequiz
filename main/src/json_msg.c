#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-glib/json-glib.h>
//#include "include/json_msg.h"	/* ->json_glib/json_glib.h; ->json_glib/json_gobject.h */

/* decode json msg from control panels */
void decode_json(char *json, char instruction[], char action[], char team[], char value[])
{
	/* new parser */
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, json, -1, NULL);

	/* new reader to read streams*/
	JsonReader *reader = json_reader_new(json_parser_get_root(parser));

	/* parse individual elements */
	json_reader_read_member(reader, "Instruction");
	strcpy(instruction, (char*)json_reader_get_string_value(reader));
	json_reader_end_member(reader);

	json_reader_read_member(reader, "Action");
	strcpy(action, (char*)json_reader_get_string_value(reader));
	json_reader_end_member(reader);

	json_reader_read_member(reader, "Team");
	strcpy(team, (char*)json_reader_get_string_value(reader));
	json_reader_end_member(reader);

	json_reader_read_member(reader, "Value");
	strcpy(value, (char*)json_reader_get_string_value(reader));
	json_reader_end_member(reader);

	/* free resources */
	g_object_unref(reader);
	g_object_unref(parser);
}

void decode_question(char *json, char **question, char **A, char **B, char **C, char **D, char **path, char **correct)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, json, -1, NULL);
	
	JsonReader *reader = json_reader_new(json_parser_get_root(parser));
	gint element_count = json_reader_count_members(reader);

	json_reader_read_member(reader, "Question");
	(*question) = (char*)malloc(sizeof(char)*strlen((char*)json_reader_get_string_value(reader))+1);
	strcpy(*question, (char*)json_reader_get_string_value(reader));
	json_reader_end_member(reader);

	json_reader_read_member(reader, "Path");
	(*path) = (char*)malloc(sizeof(char)*strlen((char*)json_reader_get_string_value(reader))+1);
	strcpy(*path, (char*)json_reader_get_string_value(reader));
	json_reader_end_member(reader);

	if(element_count>7) {
		json_reader_read_member(reader, "Options");

		json_reader_read_element(reader, 0);
		(*A) = malloc(sizeof(char)*strlen((char*)json_reader_get_string_value(reader))+1);
		strcpy(*A, (char*)json_reader_get_string_value(reader));
		json_reader_end_member(reader);

		json_reader_read_element(reader, 1);
		(*B) = malloc(sizeof(char)*strlen((char*)json_reader_get_string_value(reader))+1);
		strcpy(*B, (char*)json_reader_get_string_value(reader));
		json_reader_end_member(reader);

		json_reader_read_element(reader, 2);
		(*C) = malloc(sizeof(char)*strlen((char*)json_reader_get_string_value(reader))+1);
		strcpy(*C, (char*)json_reader_get_string_value(reader));
		json_reader_end_member(reader);

		json_reader_read_element(reader, 3);
		(*D) = malloc(sizeof(char)*strlen((char*)json_reader_get_string_value((reader))+1));
		strcpy(*D, (char*)json_reader_get_string_value(reader));
		json_reader_end_member(reader);

		json_reader_end_member(reader);
	}
	else {
		A = NULL; B = NULL; C = NULL; D = NULL;
	}

	json_reader_read_member(reader, "Answer");
	(*correct) = malloc(sizeof(char)*strlen((char*)json_reader_get_string_value((reader))+1));
	strcpy(*correct, (char*)json_reader_get_string_value(reader));
	json_reader_end_member(reader);

	/* free resources */
	g_object_unref(reader);
	g_object_unref(parser);
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
	json_builder_add_string_value(builder, instruction);

	json_builder_set_member_name(builder, "Action");
	json_builder_add_string_value(builder, action);

	json_builder_set_member_name(builder, "Team");
	json_builder_add_string_value(builder, team);

	json_builder_set_member_name(builder, "Value");
	json_builder_add_string_value(builder, value);

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
	strcpy(json_string, json);
	json_string[strlen(json)] = '\n';

	/* free resources */
	json_node_free(root);
	g_object_unref(builder);
	g_object_unref(generator);
	g_free(json);

	return json_string;
}
