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
<<<<<<< HEAD
	printf("\n\n START test.work \n\n");
=======
	//print_trie(trie->root,0);
	int pos;
	int exists;
	exists=check_exists_in_children(trie->root,"clear",&pos);
	printf("exists :%d , pos:%d\n",exists,pos);
	//printf("children of root are %d\n",trie->root->number_of_childs);
>>>>>>> 6feb454... aadd / delete fixed
	test_input(trie,"test.work");
	//printf("children of root are %d\n",trie->root->number_of_childs);
	int i;
	/*for(i=0;i<trie->root->number_of_childs;i++){
		exists=check_exists_in_children(trie->root,trie->root->children[i].word,&pos);
		if(exists==0) printf("FALSE %d %s %d %d\n",i,trie->root->children[i].word,exists,pos);
	}*/
	printf("Print trie \n");
	print_trie(trie->root,0);
	return 0;	
}
