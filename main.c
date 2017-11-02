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

//	printf("Init %s\nQueries %s\n",init_file,query_file);

	struct index *trie=malloc(sizeof(struct index));
	trie->root=init_trie();
	if(init_input(trie,init_file)<0) return -1;
	//print_trie(trie->root,0);
//	int pos;
//	int exists;
	printf("before test input\n");
	if(test_input(trie,query_file)<0)
		return -1;
	//int i;
	//int pos;
	//int exists=check_exists_in_children(trie->root,"having",&pos);
	//printf("\nexists %d pos of word is %d\n",exists,pos);
	/*char **test_words=malloc(10*sizeof(char*));
	int i;
	for(i=0;i<10;i++){
		test_words[i]=malloc(15*sizeof(char));
		if(test_words[i]==NULL)	return -1;	
	}
	strcpy(test_words[0],"the");
	strcpy(test_words[1],"1");
	int command_error=delete_ngram(trie->root,test_words,0,1);
	printf("command error is %d\n",command_error);*/
	printf("\n");
	//print_trie(trie->root,0);
	delete_trie(trie);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nProgram executed in %f seconds\n",time_spent);
	//print_trie(trie->root,0);
	return 0;	
}
