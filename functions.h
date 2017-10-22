#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define WORD_SIZE 25
#define CHILD_NUM 10
#define MAX_CHILDS 1
#define ERROR 1
#define SUCCESS 0
//#define STACK_NUMBER 10
//#define STACK_EMPTY -1

<<<<<<< HEAD
typedef struct trie_node{
	char *word;
	struct trie_node *children;
	char is_final;
	int number_of_childs;
	int max_childs;

}trie_node;

struct index{
	trie_node *root;
};

/**
typedef struct stack{
	int *pos_array;
	int top;
	int max_elements;
}stack;


stack *init_stack();
int search_in_trie(trie_node *root,char **word,int number_of_words);
int push(stack *stack_,int pos);
void print_stack(stack *stack_);
int pop(stack *stack_);
void stack_destroy(stack *stack_);
*/
void printpanos(void);
trie_node *init_trie();
int search_in_trie(trie_node *root,char **word,int number_of_words);
int check_exists_in_children(trie_node *node,char *word,int *pos);
int append_trie_node(trie_node *root,char **word,int word_number,int number_of_words);
int append_word(trie_node *node,int pos,char *word,char is_final);
void print_trie(trie_node *node,int level);
=======

int openinput(char * filename);
char *getWord(FILE *fp);
>>>>>>> ec2bf7ae61c93922e7b02d4728a9cb475f034d69
