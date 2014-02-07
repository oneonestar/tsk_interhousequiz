#ifdef JSON_STRING_H
#define JSON_STRING_H

char *json_create(char*, char*, char*, char*, char*, char*, char*);
char **json_parse_array(json_object*, char*);
char **answer_parser(char*);
char **question_parser(char*);

#endif
