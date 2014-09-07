#ifndef STACK_H
#define STACK_H

struct Stack {
	int *data;
	int pointer;
	int size;
};

void new_stack(struct Stack*);
int pop(struct Stack*);
void push(struct Stack*, int);
#endif
