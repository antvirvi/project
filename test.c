#include "test.h"


int init_test_input(struct index *trie,char * filename){
	//printf("\x1b[32m""INIT_INPUT start\n""\x1b[0m");
	int table_size = 10;
	int word_size = 25;
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
		test_add(trie,ptr_table,words_in,FOUND);
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


void test_delete(struct index *trie,char **words_to_check ,int words_size,int expected_result){
		int error=delete_ngram(trie->root,words_to_check,0,words_size-1);
		//printf("error is %d\n",error);
		int result=test_if_exists(trie,words_to_check ,words_size);
		//printf("result is %d\n",result);
		if(result!=expected_result){
			printf("Wrong result in delete\n");
		}
		
	}

void test_add(struct index *trie,char **words_to_check ,int words_size,int expected_result){
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

int parseInt (char c) {
    return c - '0';
}

int tests_from_file(struct index *trie,char * filename){
	printf("Inside tests from file\n");
	int table_size=10;
	int word_size=25;	
	char **ptr_table = malloc(table_size*sizeof(char *));
	int words_in = 0;
	int flag; //1 question, 2 addition, 3 deletion, 4 end of file
	int a;
	FILE* fd = fopen(filename, "r"); //opening input file
	if(fd == NULL)
	{
		perror("Error opening input file");
		return -1;
	}
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *word;
	int command_error;


	for(a=0;a<table_size;a++)
		ptr_table[a]=malloc(word_size*sizeof(char));

	while ((read = getline(&line, &len, fd)) != -1) {
		//words_in = 1;
		words_in = 0;
		
		word = strtok (line," \n");
		while(word!=NULL){
			if(strcmp(word,"Q")==0){
				flag=1;
			}
			else if(strcmp(word,"A")==0){
				flag=2;
			}
			else if(strcmp(word,"D")==0){
				flag=3;
			}
			else if(strcmp(word,"F")==0){
				//printf("\x1b[36m""EOF -1\n""\x1b[0m");
				/*
				cleanup(ptr_table);
				free(line);
				fclose(fd);
				printf("\x1b[32m""TEST_INPUT end\n""\x1b[0m");
				return 1;
				*/
				//printf("\x1b[32m""F -> print paths\n""\x1b[0m");	
			}else if(strcmp(word,"\0")==0){				
			}
			else{
			
				
				if(words_in==table_size-1){
					table_size*=2;
					ptr_table = realloc(ptr_table,table_size*(sizeof(char*)));
					for(a=table_size/2;a<table_size;a++){
						ptr_table[a]=malloc(word_size*sizeof(char));
					}
				}
				if(strlen(word)>=word_size){
					word_size*=2;
					for(a=0;a<table_size;a++)
						ptr_table[a] = realloc(ptr_table[a],word_size*sizeof(char));
				}		
			strcpy(ptr_table[words_in],word);
			words_in++;				
			}
			word=strtok(NULL," \n");
		}


int expected_result = parseInt(ptr_table[words_in-1][0]);  //kratasei ton teleytaio arithmo apo tin seira pou diabase
	words_in --;  									//meiwnei to words_in gia na min lifthei upopsin o teleytaios arithmos

		switch(flag){
			case 1 :
				printf("\n"); 
				break;
			case 2 :
				test_add(trie,ptr_table,words_in,expected_result);
				break;
			case 3 :
				test_delete(trie,ptr_table,words_in,expected_result);
		
		}
		flag=0;	
	}

  	free(line);
	cleanup(ptr_table);
	fclose(fd);
	
return 0;

}

void test_search(){}
