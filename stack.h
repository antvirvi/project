#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR 1
#define SUCCESS 0
#define STACK_NUMBER 10
#define STACK_EMPTY -1

typedef struct stack{
	int *pos_array;
	int top;
	int max_elements;
}stack;

stack *init_stack();
int push(stack *stack_,int pos);
int pop(stack *stack_);
void stack_destroy(stack *stack_);
void print_stack(stack *stack_);

