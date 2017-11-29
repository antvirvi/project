#include "bloomfilter.h"

#ifndef FUNC_H
#define FUNC_H
#include "functions.h"
#endif
#include "static_functions.h"
#include <errno.h>
#include <time.h>

//#define hash_length 1024
#define bitsint sizeof(int)*8


int main (int argc, char **argv ) {
	clock_t begin = clock();
	int c;
	char init_file[25];
	char query_file[25];
	
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


	for(i=0;i<6;i++){
		free(words[i]);
	}
	free(words);
	printf("Init %s\nQueries %s\n",init_file,query_file);

	struct index *trie=malloc(sizeof(struct index));
	trie->hash=createLinearHash(C,10);
	//trie->root=init_trie();
	if(init_input(trie,init_file)<0) return -1;


	if(test_input(trie,query_file)<0)
		return -1;
	printf("before test input\n");
	//print_hash(trie->hash);

	static_hash_layer *static_hash=compress(trie->hash);
	destroyLinearHash(trie->hash);
	free(trie);

	struct static_index *static_trie=malloc(sizeof(struct static_index));	
	static_trie->hash=static_hash;
	//init_static_input(static_trie,init_file);
	print_static_hash(static_hash);
	if(test_static_input(static_trie,"static_test.work")<0)
		return -1;
	
	print_static_hash(static_hash);
	destroy_static_hash(static_hash);

	free(static_trie);

	printf("\n");
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nProgram executed in %f seconds\n",time_spent);
	return 0;	
}
