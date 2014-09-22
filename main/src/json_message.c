#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <json/json.h>

int main(int argc, char **argv) {
	json_object *new_obj;
	char buf[] = "{ \"instruction\": \"Score\", \"action\": \"add\", \"option\": \"D\", \"data\": \"10\" }";
	new_obj = json_tokener_parse(buf);
	json_object *foo;
	json_object_object_get_ex(new_obj,"instruction",&foo);
	printf("The value of instruction is %s\n", json_object_get_string(foo));
	json_object_object_get_ex(new_obj,"action",&foo);
	printf("The value of action is %s\n", json_object_get_string(foo));
	json_object_object_get_ex(new_obj,"option",&foo);
	printf("The value of option is %s\n", json_object_get_string(foo));
	json_object_object_get_ex(new_obj,"data",&foo);
	printf("The value of data is %d\n", json_object_get_int(foo));
}
