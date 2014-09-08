#ifndef _SERVER_CB_H_
#define _SERVER_CB_H_
#include "link_list.h"
#include "server.h"
#include "server_cb.h"
#include "non_blocking_socket.h"
#include "layout.h"
#include "db_redis.h"

void on_read_cb(struct bufferevent*, void*);
int parse_instruction(char*);
int parse_option(int, char*);

#endif
