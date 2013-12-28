#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *json_create(char *question, char *A, char *B, char *C, char *D, char *correct, char *path)
{
	struct json_object *root = (struct json_object*)json_object_new_object();
	json_object_object_add(root, "Question", json_object_new_string(question));
	json_object_object_add(root, "A", json_object_new_string(A));
	json_object_object_add(root, "B", json_object_new_string(B));
	json_object_object_add(root, "C", json_object_new_string(C));
	json_object_object_add(root, "D", json_object_new_string(D));
	json_object_object_add(root, "Answer", json_object_new_string(correct));
	json_object_object_add(root, "Path", json_object_new_string(path));

	char *json_string = (char*)malloc(sizeof(char)*strlen((char*)json_object_get_string(root)));
	strcpy(json_string, (char*)json_object_get_string(root));
	return json_string;
}

int main(void)
{
	char *aString = json_create("test", "123", "223", "323", "423", "123", "abc");
	printf("json-string generated: %s\n", aString);
	return 0;
}
