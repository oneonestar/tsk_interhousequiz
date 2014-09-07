#include "stack.h"
#include <stdlib.h>

void new_stack(struct Stack *stack) {
	stack->data = (int*)malloc(sizeof(int)*10);
	stack->pointer = -1;
}

void push(struct Stack *stack, int value) {
	if(stack->pointer<10) {
		stack->pointer++;
		stack->data[stack->pointer] = value;
	}
}

int pop(struct Stack *stack) {
	if(stack->pointer>=0) {
		stack->pointer--;
		return stack->data[stack->pointer+1];
	}
}
