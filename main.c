#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main (int argc, char **argv )
{
	struct index *trie=malloc(sizeof(struct index));
	trie->root=init_trie();
	openinput(trie,"input.txt");
	print_trie(trie->root,0);
	return 0;	
}
