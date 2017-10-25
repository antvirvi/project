#include "stack.h"

stack *init_stack(){
	stack *stack_=malloc(sizeof(struct stack));
	if(stack_==NULL) return NULL;

	stack_->pos_array=malloc(STACK_NUMBER*sizeof(int));
	if(stack_->pos_array==NULL) return NULL;
	stack_->top=0;
	stack_->max_elements=STACK_NUMBER;
	return stack_;
}

int push(stack *stack_,int pos){
	if(stack_->top==stack_->max_elements){
		stack_->pos_array=realloc(stack_->pos_array,2*stack_->max_elements*sizeof(int));
		if(stack_->pos_array==NULL)	return ERROR;
	}
	int free_pos=stack_->top;
	stack_->pos_array[free_pos]=pos;
	stack_->top++;
	return SUCCESS;
}

int pop(stack *stack_){
	if(stack_->top==0) return STACK_EMPTY;
	int pos=stack_->pos_array[stack_->top];
	stack_->top--;
	return pos;

}

void reset_stack(stack *stack_){
	stack_->top=0;
}

void stack_destroy(stack *stack_){
	free(stack_->pos_array);
	stack_->pos_array=NULL;
	stack_->top=-1;
	free(stack_);

}

void print_stack(stack *stack_){
	int i;
	printf("Inside print stack\n");
	for(i=0;i<stack_->top;i++){
		printf("%d ",stack_->pos_array[i]);
	}
	printf("\n");
}

