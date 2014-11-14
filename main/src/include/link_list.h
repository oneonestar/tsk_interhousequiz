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

#ifndef _LINK_LIST_H_
#define _LINK_LIST_H_

/**
 * @brief Structure for storing each connection.
 */
typedef struct Node {
	struct bufferevent *bev;
	struct Info *inf;
	struct Node *next;
} node;



/**
 * @brief A list storing all the connections.
 */
typedef struct List {
	node *start, *end;
	int size;
} list;


struct List *theList;

/**
 * @brief Searching the connection using the address.
 * and port.
 * @param bev the bufferevent send to the callback function.
 * @param theList the linked list containin the connections.
 * @retval return null if not found.
 */
node *listSearch(const struct bufferevent *bev, const list *theList);

/**
 * @brief Create and initialize the linked list.
 * @param list the bufferevent send to the callback function.
 */
void listCreate(struct List **list);

/**
 * @brief Create and add a new node into the list.
 * @param bev the bufferevent bounded to that connection.
 * @param info the info structure bounded to that connection.
 * @param list the bufferevent send to the callback function.
 */
void listAdd(struct bufferevent* bev, struct Info* info, struct List *list);

/**
 * @brief Remove the node from the list.
 * @param node The node needed to be removed from the list.
 * @param list The List structure which containing the connections.
 */
void listRemove(struct Node *node, struct List *list);

/**
 * @brief Print the node to wprintw().
 * @param list The List structure which containing the connections.
 */
void listPrint(struct List *list);

/**
 * @brief Delete all the nodes in the list.
 * @param list The List structure which containing the connections.
 */
void listDelete(struct List *list);

/**
 * @brief Broadcast the msg to all connections in the list.
 * @param list the List structure which containing the connections.
 * @param msg the message wanted to broadcast.
 */
int listBroadcast(list*, char*);

#endif
