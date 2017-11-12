#include "functions.h"
#include <errno.h>
#include <time.h>


int main (int argc, char **argv )
{	clock_t begin = clock();
	int c;
	char init_file[16];
	char query_file[16];
	
	if(argc<5){
		printf("Wrong quantity of arguments\n");
		return -1;
	}
	

	for(c=0;c<argc;c++)
	{
		if(strcmp(argv[c],"-i")==0)
			strcpy(init_file,argv[c+1]);
		if(strcmp(argv[c],"-q")==0)
			strcpy(query_file,argv[c+1]);	
	}
	
	int i;
	char **words=malloc(6*sizeof(char*));
	for(i=0;i<6;i++){
		words[i]=malloc(15*sizeof(char));
	}
	strcpy(words[0],"test");
	strcpy(words[1],"this");
	strcpy(words[2],"is");
	strcpy(words[3],"a");
	strcpy(words[4],"cat");
	hash_layer *hash=createLinearHash(4,1);
	insertTrieNode(hash,words,5);
	insertTrieNode(hash,&(words[3]),2);
	insertTrieNode(hash,words,1);
	insertTrieNode(hash,words,2);
	insertTrieNode(hash,words,4);
	print_hash(hash);
//	printf("Init %s\nQueries %s\n",init_file,query_file);

	/*struct index *trie=malloc(sizeof(struct index));
	trie->root=init_trie();
	if(init_input(trie,init_file)<0) return -1;

	printf("before test input\n");
	if(test_input(trie,query_file)<0)
		return -1;
	

	printf("\n");
	//print_trie(trie->root,0);
	delete_trie(trie);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nProgram executed in %f seconds\n",time_spent);
	//print_trie(trie->root,0);*/
	return 0;	
}
