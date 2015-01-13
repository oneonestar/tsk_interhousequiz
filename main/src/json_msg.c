#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/json_msg.h"	/* ->json_glib/json_glib.h; ->json_glib/json_gobject.h */

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

	/* free resources */
	json_node_free(root);
	g_object_unref(builder);
	g_object_unref(generator);

	return json;
}

/*
int main(int argc, char **argv)
{
	char buf[] = "{ \"Instruction\": \"Score\", \"Action\": \"add\", \"Team\": \"A\", \"Value\": \"10\" }";
	char inst[10], act[10], team[2], val[10];
	decode_json(buf, inst, act, team, val);
	printf("%s,%s,%c,%s\n", inst,act,team[0],val);
	char *json = encode_json("hello", "world", "D", "world", 1);
	decode_json(buf, inst, act, team, val);
	printf("%s\n", json);
}
*/
