#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

#ifndef FUNC_H
#define FUNC_H
#include "functions.h"

#endif

struct trie_node;

typedef struct ngrams_to_delete{
	int nodes_capacity;
	int length_capacity;
	int *length;
	int pos;
	struct trie_node **nodes_to_delete;
	int *children_to_delete;
}ngrams_to_delete;




