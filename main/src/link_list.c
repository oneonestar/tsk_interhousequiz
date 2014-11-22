/***********************************************************************
 * Copyright (C) 2013-2014, Steven Chien <steven.chien@connect.polyu.hk>
 * Copyright (C) 2014, Star Poon <oneonestar@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

/**
 * @file link_list.h
 * @brief Linked list for storing the current connections.
 * @author Steven Chien
 * @author Star Poon (oneonestar@gmail.com)
 * @version 1.0
 * @date 2013-2014
 * @copyright GNU General Public License v3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include "include/link_list.h"
#include "include/layout.h"
#include "include/non_blocking_socket.h"

node *listSearch(const struct bufferevent *bev, const list *theList)
{
	node *p;
	for(p=theList->start; p != NULL; p=p->next) {
		if(p->bev == bev)
			return p;
	}
	return NULL;	/* return NULL if not found */
}

void listCreate(list **theList)
{
	*theList = (list*)malloc(sizeof(list));	/* allocate space for struct List */

	if(*theList == NULL) {
		wprintw(msg_content, "\nMemory error! Abort\n");
		exit(-1);
	}

	(*theList)->start = NULL;
	(*theList)->size = 0;

	wrefresh(msg_content);
}

/* function to add a word to list */
void listAdd(struct bufferevent *bev, struct Info *inf, list *theList)
{
	node *p = (node*)malloc(sizeof(node));				/* create new node for word */

	if(p == NULL) {
		wprintw(msg_content, "\nMemory error! Abort\n");
		exit(-1);
	}

	p->bev = bev;
	p->inf = inf;

	theList->size++;
	p->next = theList->start;
	theList->start = p;

	wrefresh(msg_content);
}

void listRemove(node *q, list *theList)
{
	if(q == NULL || theList->start == NULL)
		return;

	struct Node *temp = theList->start;
	if(theList->start == q)	{	/* delete head */
		theList->start = temp->next;
		temp = NULL;	
	}

	while(temp!=NULL) {
		if(temp->next == q) {
			temp->next = temp->next->next;
			break;
		}
		temp = temp->next;
	}

	theList->size--;
	free(q);
}

/* function for list printing */
void listPrint(list *theList)
{
	node *p;
	werase(client_content);
	wrefresh(msg_content);
	wprintw(client_content, "%-20s\tPort\n", "Address");
	wprintw(client_content, "---------------------------------------\n");

	for(p=theList->start; p!=NULL; p=p->next) {
		wprintw(client_content, "%-20s\t%d\n", get_address_string(p->inf), get_port_int(p->inf));
		wrefresh(client_content);
	}

	wprintw(client_content, "Connected Clients: %d\n", theList->size);
	wrefresh(client_content);
	wrefresh(msg_content);
}

/* function for list deletion */
void listDelete(list *theList)
{
	node *p = theList->start;

	while(p!=NULL) {
		node *temp = p->next;
		free(p);
		p = temp;
	}
	theList->start = NULL;
}

int listBroadcast(list *theList, char *msg)
{
	node *p;

	wprintw(msg_content, "Broadcast msg: %s\n", msg);
	for(p=theList->start; p!=NULL; p=p->next) {
		wprintw(msg_content, "client: %s:%d\n", get_address_string(p->inf), get_port_int(p->inf));
		bufferevent_write(p->bev, msg, strlen(msg)+1);
	}

	wrefresh(msg_content);
	return 0;
}
