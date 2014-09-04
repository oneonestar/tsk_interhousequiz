#ifndef _LINK_LIST_H_
#define _LINK_LIST_H_

//type definition for structure node
typedef struct Node {
	struct bufferevent *bev;
	struct info *inf;
	char *words;
	int count;
	struct Node *next;
} node;

//type definition for structure list
typedef struct List {
	node *start, *end;
	int size;
} list;

int cmp(struct Node*, struct Node*);
void list_sort(struct List*);
node *listSearch(char[], char[], list*);
void listCreate(list**);
void listAdd(struct bufferevent*, struct info*, list*);
void listRemove(node*, list*);
void listPrint(list*);
void listDelete(list*);

#endif
