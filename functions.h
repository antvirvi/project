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

#define PATH_COLUMN 10
//#define STACK_NUMBER 10
//#define STACK_EMPTY -1

typedef struct trie_node{
	char static_word[WORD_SIZE];
	char *word;
	struct trie_node *children;
	char is_final;
	int number_of_childs;
	int max_childs;

}trie_node;

struct index{
	trie_node *root;
};


typedef struct paths{

	int **paths_array;//[10][10];
	int words_in;
	int max_words;
}paths;

void cleanup2(char ** ptr);
void printtable(char ** pt,int num);
void printpanos(void);
trie_node *init_trie();
void delete_trie(struct index *trie);
void destroy_childs(trie_node *node);
int delete_ngram(trie_node *root,char **word,int word_number,int number_of_words);
int search_in_trie(trie_node *root,char **word,int number_of_words);
int check_exists_in_children(trie_node *node,char *word,int *pos);
int append_trie_node(trie_node *root,char **word,int word_number,int number_of_words);
int delete_from_node(trie_node *node,int pos);
int delete_ngram(trie_node *root,char **word,int word_number,int number_of_words);
int append_word(trie_node *node,int pos,char *word,char is_final);
int append_trie_node_iterative(trie_node *root,char **word,int word_number,int number_of_words);
void print_trie(trie_node *node,int level);
void print_nodes_from_stack(trie_node *root,stack *stack_);
int init_input(struct index *trie,char * filename);
int test_input(struct index *trie,char * filename);

int double_paths(paths *paths_);
void delete_paths(struct paths *paths_);
int check_in_paths(struct paths *paths_, stack *stack_,trie_node *node);
void add_to_paths(struct paths *paths_, stack *stack_);

int check_in_paths3(struct paths *paths_, stack *stack_,trie_node *node);
void print_paths(paths *paths_);
//void add_to_paths2(struct paths *paths_, stack *stack_);
struct paths *init_paths(int rows,int columns);

void cleanup(char ** ptr);
