#ifndef _NON_BLOCKING_SOCKET_H_
#define _NON_BLOCKING_SOCKET_H_

#include <event2/util.h>
#include <event2/listener.h>

/**
 * @brief Structure for storing the IP and address in char*
 */
struct Info {
	struct sockaddr_storage address;
	int total_drained;
};

char* get_address_string(const struct Info*);
int get_port_int(const struct Info*);

void on_event_cb(struct bufferevent*, short, void*);
void on_read_cb(struct bufferevent*, void*);
void on_accept_cb(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int, void*);
void on_accept_errorcb(struct evconnlistener *listener, void *ptr);

#endif
