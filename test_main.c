#include "functions.h"
#include <errno.h>
#include <time.h>


int main (int argc, char **argv )
{
	int i;
	struct index *trie=malloc(sizeof(struct index));
	trie->root=init_trie();

	char **test_words=malloc(10*sizeof(char*));
	for(i=0;i<10;i++){
		test_words[i]=malloc(15*sizeof(char));
		if(test_words[i]==NULL)	return -1;	
	}

	strcpy(test_words[0],"panos");
	test_add(trie,test_words,1);
	tests_for_binary(trie);

	//print_trie(trie->root,0);
	delete_trie(trie);
	
	return 0;	
}
