#ifndef _SERVER_CB_H_
#define _SERVER_CB_H_

void on_read_cb(struct bufferevent*, void*);
int parse_instruction(char*);
int parse_option(int, char*);
void hash_table_init();

#endif
