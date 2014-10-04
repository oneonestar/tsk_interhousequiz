#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <json/json.h>

void decode_msg(char *json, char instruction[], char action[], char *team, char value[]) {

	// create json objects and parse json string
	json_object *json_obj = json_tokener_parse(json);
	json_object *json_element;

	// get instruction
	json_object_object_get_ex(json_obj, "Instruction", &json_element);
	strcpy(instruction, json_object_get_string(json_element));

	// get action
	json_object_object_get_ex(json_obj, "Action", &json_element);
	strcpy(action, json_object_get_string(json_element));

	// get team
	json_object_object_get_ex(json_obj, "Team", &json_element);
	strcpy(team,json_object_get_string(json_element));

	// get value
	json_object_object_get_ex(json_obj, "Value", &json_element);
	strcpy(value, json_object_get_string(json_element));

}

char *encode_msg(char *instruction, char *action, char *team, char *value) {

	json_object *jobj = json_object_new_object();
	json_object_object_add(jobj, "Instruction", json_object_new_string(instruction));
	json_object_object_add(jobj, "Action", json_object_new_string(action));
	json_object_object_add(jobj, "Team", json_object_new_string(team));
	json_object_object_add(jobj, "Value", json_object_new_string(value));

	char *json = (char*)malloc(sizeof(char)*strlen(json_object_get_string(jobj))+1);
	strcpy(json, json_object_get_string(jobj));
	return json;
}

int main(int argc, char **argv) {
	char buf[] = "{ \"Instruction\": \"Score\", \"Action\": \"add\", \"Team\": \"A\", \"Value\": \"10\" }";
	char inst[10], act[10], team[2], val[10];
	decode_msg(buf, inst, act, team, val);
	printf("%s,%s,%c,%s\n", inst,act,team[0],val);
	char *msg = encode_msg("hello", "world", "D", "world");
	printf("%s\n", msg);
}
