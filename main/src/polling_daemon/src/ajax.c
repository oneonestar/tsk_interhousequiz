#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "ajax.h"

struct string {
	char *ptr;
	size_t len;
};

void init_string(struct string *s) {
	s->len = 0;
	s->ptr = (char*)malloc(sizeof(char)*(s->len+1));
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(1);
	}
	s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc((char*)s->ptr, new_len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(1);
	}
	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}


int ajax() {

	curl_global_init(CURL_GLOBAL_ALL);
	CURL * myHandle;
	CURLcode result; // We’ll store the result of CURL’s webpage retrieval, for simple error checking.
	myHandle = curl_easy_init() ;
	struct string s;
	init_string(&s);
	 
	curl_easy_setopt(myHandle, CURLOPT_URL, address);
	curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, &s);
	curl_easy_setopt(myHandle, CURLOPT_BUFFERSIZE, 128);
	curl_easy_setopt(myHandle, CURLOPT_USERPWD, "root:arduino"); 
	result = curl_easy_perform(myHandle); 

	curl_easy_cleanup(myHandle);
	printf("%s\n", s.ptr);
	free(s.ptr);

	return 0;
}
