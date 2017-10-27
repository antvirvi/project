#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main (int argc, char **argv )
{	
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
	if(!init_input(trie,init_file))
		return -1;
	//print_trie(trie->root,0);
	int pos;
	int exists;
	//printf("\n\n\nSECOND Stage~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
	//exists=check_exists_in_children(trie->root,"clear",&pos);
	//printf("exists :%d , pos:%d\n",exists,pos);
	//printf("children of root are %d\n",trie->root->number_of_childs);
	if(!test_input(trie,query_file))
		return -1;
	//printf("children of root are %d\n",trie->root->number_of_childs);
	//int i;
	/*for(i=0;i<trie->root->number_of_childs;i++){
		exists=check_exists_in_children(trie->root,trie->root->children[i].word,&pos);
		if(exists==0) printf("FALSE %d %s %d %d\n",i,trie->root->children[i].word,exists,pos);
	}*/
//	printf("Print trie \n");
//=======
	//int i;
	//printf("Print trie \n");
	//print_trie(trie->root,0);
	return 0;	
}
