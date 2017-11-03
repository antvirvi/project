#include "test.h"


int init_test_input(struct index *trie,char * filename){
	//printf("\x1b[32m""INIT_INPUT start\n""\x1b[0m");
	int table_size=10;
	int word_size=25;
	int a;
	char **ptr_table = malloc(table_size*sizeof(char *));
	int words_in = 0;

	FILE* fd = fopen(filename, "r"); //opening input file
	//strcpy(buffer,"\0");

	if(fd == NULL){
		perror("Error opening input file");
		return -1;
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *word;

	for(a=0;a<table_size;a++)
		ptr_table[a]=malloc(word_size*sizeof(char));
			
	while ((read = getline(&line, &len, fd)) != -1){
		words_in = 0;
		word = strtok (line," \n");
		while(word!=NULL)
		{
		//printf("Read this word: %s\n",word);
			if(words_in==table_size){
				table_size*=2;
				ptr_table = realloc(ptr_table,table_size*sizeof(char*));
				for(a=(table_size/2);a<table_size;a++)
					ptr_table[a]=malloc(word_size*sizeof(char));
			}
			if(strlen(word)>word_size){
				word_size*=2;
				for(a=0;a<table_size;a++)
					ptr_table[a] = realloc(ptr_table[a],word_size*sizeof(char));
			}
		//	ptr_table[words_in] = malloc(word_size*sizeof(char));
			strcpy(ptr_table[words_in],word);
			//printf("Kuru word = %s\n",ptr_table[words_in]);
			words_in++;
			word=strtok(NULL," \n");
		}
		test_add(trie,ptr_table,words_in);
	}
	//printf ("free\n");
	free(line);
	cleanup(ptr_table);
	fclose(fd);

	//printf("\x1b[32m""INIT_INPUT end\n""\x1b[0m");
	return 0;	
}


int test_if_exists(struct index *trie,char **words ,int words_size){
	//printf("Inside check if exists\n");
	int i=0;
	trie_node *node =trie->root;
	int found=NOT_FOUND;
	int pos;
	while(i!=words_size){
		found=check_exists_in_children(node,words[i],&pos);
		if(found==NOT_FOUND) break;
		node=&(node->children[pos]);
		i++;  
		}
		if(found==FOUND && node->is_final=='n') found=NOT_FOUND; 
		return found;
}


void test_delete(struct index *trie,char **words_to_check ,int words_size){
		if(test_if_exists(trie,words_to_check ,words_size)!=NOT_FOUND){
			printf("Ngram was not deleted\n");
		}
		
	}

void test_add(struct index *trie,char **words_to_check ,int words_size){
		append_trie_node(trie->root,words_to_check,0,words_size-1);
		int found=test_if_exists(trie,words_to_check ,words_size);
		if(found!=FOUND) printf("Ngram was not added\n");
	}

void test_binary_search(struct index *trie,char *word,int expected_found,int expected_position){
	int found;
	int pos;
	found=check_exists_in_children(trie->root,word,&pos);
	if(found!=expected_found){ printf("Wasn't found\n");return;}
	if(pos!=expected_position && found==1){ printf("%s Wasn't found in the right position\n",word);return;}
}

void tests_for_binary(struct index *trie){
	char *test_word=malloc(15*sizeof(char));
	//some tests
	strcpy(test_word,"0000");
	test_binary_search(trie,test_word,0,0);
	
	strcpy(test_word,"zzz");
	test_binary_search(trie,test_word,0,trie->root->number_of_childs-1);

	free(test_word);
}

void test_search(){}
