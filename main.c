#include "functions.h"
#include <errno.h>
#include <time.h>

//#define hash_length 1024
#define bitsint sizeof(int)*8

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

	printf("before test input\n");
	if(test_input(trie,query_file)<0)
		return -1;
	

	printf("\n");
	//print_trie(trie->root,0);
	delete_trie(trie);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nProgram executed in %f seconds\n",time_spent);
	//print_trie(trie->root,0);
unsigned long hs;
	hs = hash((unsigned char *)"antonsi vrivilis",1);
	printf("--> %ld\n",hs);
	hs = hash((unsigned char *)"antonsi vrivilis",2);
	printf("--> %ld\n",hs);
	hs = hash((unsigned char *)"antonsi vrivilis",3);
	printf("--> %ld\n",hs);
	hs = hash((unsigned char *)"antonsi vrivilis",4);
	printf("--> %ld\n",hs);
	hs = hash((unsigned char *)"antonsi vrivilis",5);
	printf("--> %ld\n",hs);
	hs = hash((unsigned char *)"antonsi vrivilis",6);
	printf("--> %ld\n",hs);
	hs = hash((unsigned char *)"antonsi vrivilis",7);
	printf("--> %ld\n",hs);
	hs = hash((unsigned char *)"antonsi vrivilis",8);
	printf("--> %ld\n",hs);


printf("Marahia Carrey %lu\n",bitsint);
//int * array[(M/sizeof(int))/8];
//init_bloomfilter(array);
	return 0;	
}
