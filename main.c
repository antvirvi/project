#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main (int argc, char **argv )
{
	struct index *trie=malloc(sizeof(struct index));
	trie->root=init_trie();
	init_input(trie,"test.init");
	printf("\n\n START test.work \n\n");
	test_input(trie,"test.work");
	printf("Print trie\n");
	print_trie(trie->root,0);
	return 0;	
}
