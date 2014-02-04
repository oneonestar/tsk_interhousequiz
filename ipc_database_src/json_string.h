#ifdef JSON_STRING_H
#define JSON_STRING_H

wchar_t *json_create(wchar_t*, wchar_t*, wchar_t*, wchar_t*, wchar_t*, wchar_t*, wchar_t*);
wchar_t **json_parse_array(json_object*, wchar_t*);
wchar_t **answer_parser(wchar_t*);
wchar_t **question_parser(wchar_t*);

#endif
