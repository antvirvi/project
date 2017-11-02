#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


#define FOUND 1
#define NOT_FOUND 0

int test_if_exists(struct index *trie,char **words_to_check ,int words_size);


void test_delete(struct index *trie,char **words_to_check ,int words_size);

void test_add(struct index *trie,char **words_to_check ,int words_size);

void test_search();
void test_add(struct index *trie,char **words_to_check ,int words_size);
void test_binary_search(struct index *trie,char *word,int expected_found,int expected_position);
void test_binary_search(struct index *trie,char *word,int expected_found,int expected_position);
void tests_for_binary(struct index *trie);
