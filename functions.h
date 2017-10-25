#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#ifndef WORD_SIZE
#define WORD_SIZE 25
#endif

#ifndef CHILD_NUM
#define CHILD_NUM 10
#endif

#ifndef MAX_CHILDS
#define MAX_CHILDS 10
#endif

#ifndef ERROR
#define ERROR 1
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif
//#define STACK_NUMBER 10
//#define STACK_EMPTY -1

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
int delete_from_node(trie_node *node,int pos);
void printtable(char ** pt,int num);
void printpanos(void);
trie_node *init_trie();
int delete_ngram(trie_node *root,char **word,int word_number,int number_of_words);
int search_in_trie(trie_node *root,char **word,int number_of_words);
int check_exists_in_children(trie_node *node,char *word,int *pos);
int append_trie_node(trie_node *root,char **word,int word_number,int number_of_words);
int append_word(trie_node *node,int pos,char *word,char is_final);
void print_trie(trie_node *node,int level);
int init_input(struct index *trie,char * filename);
int test_input(struct index *trie,char * filename);
void cleanup(char ** ptr);
