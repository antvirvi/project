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
	//print_trie(trie->root,0);
	int pos;
	int exists;
	printf("\n\n\nSECOND Stage~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
	//exists=check_exists_in_children(trie->root,"clear",&pos);
	//printf("exists :%d , pos:%d\n",exists,pos);
	//printf("children of root are %d\n",trie->root->number_of_childs);
	test_input(trie,"test.work");
	//int i;
	//printf("Print trie \n");
	print_trie(trie->root,0);
	return 0;	
}
