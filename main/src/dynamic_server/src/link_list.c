/********************************************************************************/
/*Program name: iCountApp							*/
/*Author: Chien Wei Der								*/
/*Date of Completion: 12 Mar 2013						*/
/*Description: this is a program written in accordance to the requirement of	*/
/*             HKPU COMP305 2013 Assignment 1 "iCountApp".			*/
/*										*/
/*	Copyright CHIEN, Wei Der 2013						*/
/*										*/
/*	This program is free software: you can redistribute it and/or modify	*/
/*	it under the terms of the GNU General Public License as published by	*/
/*	the Free Software Foundation, either version 3 of the License, or	*/
/*	(at your option) any later version.					*/
/*										*/
/*	This program is distributed in the hope that it will be useful,		*/
/*	but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*	GNU General Public License for more details.				*/
/*										*/
/*	You should have received a copy of the GNU General Public License	*/
/*	along with this program.  If not, see <http://www.gnu.org/licenses/>.	*/
/*										*/
/********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "link_list.h"
#include "server.h"
#include "layout.h"

//comparasion for the sorting function
int cmp(struct Node *a, struct Node *b) {

	return b->count - a->count;
}

//sorting function for the linked list
void list_sort(struct List *list) {					//merge sort

	struct Node *p, *q, *e, *head, *tail;
	int insize, nmerges, psize, qsize, i;

	head = list->start;

	if (!head)
		return;

	insize = 1;

	while (1) {
		p = head;
		head = NULL;
		tail = NULL;

		nmerges = 0;

		while (p) {
			nmerges++;

			q = p;
			psize = 0;
			for (i = 0; i < insize; i++) {

				psize++;
				q = q->next;
				if (!q) break;
			}

			qsize = insize;

			while (psize > 0 || (qsize>0 && q)) {

				if (psize == 0) {

					e = q;
					q = q->next;
					qsize--;
				}
				else if (qsize == 0 || !q) {
					e = p;
					p = p->next;
					psize--;
				}
				else if (cmp (p, q) <= 0) {
					e = p;
					p = p->next;
					psize--;
				} else {
					e = q;
					q = q->next;
					qsize--;
				}

				if (tail)
					tail->next = e;
				else
					head = e;
				tail = e;
			}

			p = q;
		}
		tail->next = NULL;
		if (nmerges <= 1) {
			list->start = head;
			return;
		}
	insize *= 2;
	}
}

//list searching algorithm to see if word is already recorded
node *listSearch(char address[], char port[], list *theList) {

	node *p;
	int i;
	for(i=0, p=theList->start; i<theList->size; i++, p=p->next) {				//search from start of list
		if(strcmp(address, p->inf->address)==0 && strcmp(port, p->inf->port)==0)
			return p;						//return node if found
	}
	return NULL;								//return NULL if not found
}

//function to create a new "list"
void listCreate(list **theList) {
	
	(*theList) = (list*)malloc(sizeof(list));				//space allocation
	if((*theList)==NULL) {
		printf("\nMemory error! Abort\n");
		exit(-1);
	}
	(*theList)->start = NULL;						//initialize start
	(*theList)->end = NULL;							//initialize end
	(*theList)->size = 0;							//initialize list variables
}

//function to add a word to list
void listAdd(struct bufferevent *bev, struct info *inf, list *theList) {

	node *p = (node*)malloc(sizeof(node));				//create new node for word
	if(p==NULL) {
		printf("\nMemory error! Abort\n");
		exit(-1);
	}

	p->bev = bev;
	p->inf = inf;

	theList->size++;						//record new size of list

	if(theList->end==NULL) {					//check if word is 1st element
		theList->start = theList->end = p;
		printf("add to empty list\n");
	}
	else {
		theList->end->next = p;
		theList->end = p;
		printf("add to end\n");
	}
}

void listRemove(node *q, list *theList) {

	node *p;
	int i;
	if(q==theList->start) {
		theList->start = theList->start->next;
	}
	else {
		for(i=0, p=theList->start; i<theList->size; i++,p=p->next) {
			if(p->next==q) {
				p->next = q->next;
				break;
			}
		}
	}
	theList->size--;
	if(theList->size==0) {
		theList->start = theList->end = NULL;
	}
	free(q);
}

//function for list printing
void listPrint(list *theList) {

	node *p;
	int i;
	werase(client_list);
	box(client_list, 0, 0);
	//mvwprintw(client_list, 0, 15, "Client List");
	cli_h++;
	printf("No.\tAddress\t\tPort\n");
	//mvwprintw(client_list, cli_h, cli_w, "No.\tAddress\t\tPort");
	cli_h++;
	//mvwprintw(client_list, cli_h, cli_w, "---------------------------------------");
	printf("---------------------------------------\n");
	cli_h++;
	for(i=1,p=theList->start; i<=theList->size; i++,p=p->next) {
		printf("%d\t%s\t\t%s\n", i, p->inf->address, p->inf->port);
		//mvwprintw(client_list, cli_h, cli_w, "%d\t%s\t\t%s", i, p->inf->address, p->inf->port);
		cli_h++;
	}
	printf("Connected Clients: %d\n", theList->size);
	//wrefresh(client_list);
	cli_h = 0;
}

//function for list deletion
void listDelete(list *theList) {

	int i = 0;
	node *p = theList->start;

	while(p!=NULL) {
		node *temp = p->next;
		free(p);
		p = temp;
	}
}
