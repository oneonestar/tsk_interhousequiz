#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json/json.h>

char *json_create(char *question, char *A, char *B, char *C, char *D, char *correct, char *path)
{
	//new json structures
	struct json_object *root;
	struct json_object *option;

	//create new json objects
	root = json_object_new_object();
	option = json_object_new_array();

	//add question as string
	json_object_object_add(root, "Question", json_object_new_string(question));

	//add options to array
	json_object_array_add(option, json_object_new_string(A));
	json_object_array_add(option, json_object_new_string(B));
	json_object_array_add(option, json_object_new_string(C));
	json_object_array_add(option, json_object_new_string(D));

	//add option array to object
	json_object_object_add(root, "Options", option);

	//add answer and path to object
	json_object_object_add(root, "Answer", json_object_new_string(correct));
	json_object_object_add(root, "Path", json_object_new_string(path));

	//new string to store the json string
	char *json_string = (char*)malloc(sizeof(char)*strlen((char*)json_object_get_string(root)));
	strcpy(json_string, (char*)json_object_get_string(root));

	//return string pointer
	return json_string;
}

//array parser
char **json_parse_array( json_object *jobj, char *key) {
	void json_parse(json_object * jobj); /*Forward Declaration*/
	enum json_type type;

	//get the array
	json_object *jarray = jobj; /*Simply get the array*/
	if(key) {
		jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
	}

	int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/

	//temp variables
	int i;
	json_object * jvalue;

	//to hold the return
	char **options = (char**)malloc(sizeof(char*)*4);

	//iterate through the array and read value
	for (i=0; i< arraylen; i++) {
		jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
		options[i] = (char*)malloc(sizeof(char)*strlen(json_object_get_string(jvalue)));
		strcpy(options[i], json_object_get_string(jvalue));
	}

	return options;
}

//parse answers
char **answer_parser(char *json_string)
{
	//new tokener
	json_object *jobj = json_tokener_parse(json_string);
	enum json_type type;
	int i = 0;
	char **answer;

	json_object_object_foreach(jobj, key, val) {
		//only get the option part
		if(strcmp(key, "Options")==0) {
			answer = json_parse_array(jobj, key);
		}
	}
	return answer;
}

char **question_parser(char *json_string)
{
	json_object *jobj = json_tokener_parse(json_string);
	enum json_type type;
	char **question = (char**)malloc(sizeof(char*)*4);
	int i = 0;

	json_object_object_foreach(jobj, key, val) {
		type = json_object_get_type(val);
		switch (type) {
			case json_type_array:
				break;
			case json_type_string:
				question[i] = (char*)malloc(sizeof(char)*strlen(json_object_get_string(val)));
				strcpy(question[i], json_object_get_string(val));
				i++;
				break;
		}
	}

	return question;
}

int main(void)
{
	//dummy: create a new json string
	char *aString = json_create("test", "123", "223", "323", "423", "123", "abc");
	printf("json-string generated: %s\n", aString);

	//dummy: create parse the quesion, answer and path
	char **question = question_parser(aString);
	printf("Question\n");
	for(int i=0; i<3; i++) {
		printf("%s\n", question[i]);
	}

	//dummy: create parse the options
	char **answer = answer_parser(aString);
	printf("Answer\n");
	for(int i=0; i<4; i++) {
		printf("%s\n", answer[i]);
	}
	return 0;
}
