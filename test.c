#include "test.h"


int init_test_input(struct index *trie,char * filename,char *command){
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
		if(strcmp("add",command)==0) test_add(trie,ptr_table,words_in,FOUND);
		else deleteTrieNode(trie->hash,ptr_table,words_in-1);
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
	int i;
	trie_node *node;
	int found=NOT_FOUND;
	int pos;

	int hash_val=hash_function(trie->hash,words[0]);
	hash_bucket *bucket=&(trie->hash->buckets[hash_val]);
	found=check_exists_in_bucket(bucket,words[0],&pos);
	if(found==0) return found; //not found
	node=&(bucket->children[pos]);
	
	i=1; //keep checking
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
		int error=deleteTrieNode(trie->hash,words_to_check,words_size-1);
		//printf("error is %d\n",error);
		int result=test_if_exists(trie,words_to_check ,words_size);
		//printf("result is %d\n",result);
		if(result!=expected_result){
			printf("Wrong result in delete\n");
		}
		
	}

void test_add(struct index *trie,char **words_to_check ,int words_size,int expected_result){
		insertTrieNode(trie->hash,words_to_check,words_size);
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

void test_hash_function(struct index *trie,char * filename){

	init_test_input(trie,filename, "add");
	init_test_input(trie,filename, "delete");
	int empty=check_if_empty(trie->hash);
	if(empty==1){
		printf("All found and removed correctly\n");
		return ;
	}
	printf("Not removed correctly\n");
	return ;


}


int check_if_empty(hash_layer *hash){
	int i;
	hash_bucket *bucket;
	for(i=0;i<hash->buckets_number;i++){
		bucket=&(hash->buckets[i]);
		if(bucket->children_number>0) return 0;
	}
	return 1;
}


//-------------------------------------------------unit testing in static functions -----------------------------------------//




int test_if_exists_static(struct static_index *trie,char **words ,int words_size){
	//printf("Inside check if exists\n");
	int i;
	char *temp_word=malloc(WORD_SIZE*sizeof(char));
	
	static_trie_node *node;
	int found=NOT_FOUND;
	int pos;

	int hash_val=static_hash_function(trie->hash,words[0]);
	static_hash_bucket *bucket=&(trie->hash->buckets[hash_val]);
	found=check_exists_in_static_bucket(bucket,words[0],&pos);
	if(found==0) return found; //not found
	node=&(bucket->children[pos]);
	
	i=1; //keep checking
	int node_word=1;
	while(i!=words_size){
		if(node_word==node->number_of_words){
			found=check_exists_in_static_children(node,words[i],&pos);
			if(found==NOT_FOUND) break;
			node=&(node->children[pos]);
			i++;
			node_word=1;
			}
		else {
			get_i_word(node,node_word,temp_word);
			
			if(strcmp(temp_word,words[i])==0) found=FOUND;
			else found=NOT_FOUND; 
			i++;
			node_word++;
		} 
		}
		if(found==FOUND && node->is_final[node_word-1]>0) found=NOT_FOUND;
		free(temp_word);
		return found;
}

int check_number_of_childs(static_trie_node *node){
	if(node->number_of_childs==1) return 1;
	int i,error;
	for(i=0;i<node->number_of_childs;i++){
		error=check_number_of_childs(&(node->children[i]));
		if(error==1) return 1;
	}
	return 0;
}

void test_compress(static_hash_layer *hash){
	int i,j;
	static_hash_bucket *bucket;
	for(i=0;i<hash->buckets_number;i++){
		bucket=&(hash->buckets[i]);
		for(j=0;j<bucket->children_number;j++){
			int error=check_number_of_childs(&(bucket->children[j]));
			if(error == 1){
				printf("Compress failed\n");
				return;
			}
		}
	}
	printf("compress succeded\n");
	return;
}

void test_everything_exists(struct static_index *trie,char * filename){
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
	int result;
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
		result=test_if_exists_static(trie,ptr_table,words_in);
		if(result==NOT_FOUND){
			printf("FAILURE:Something is missing from trie\n");
			free(line);
			cleanup(ptr_table);
			fclose(fd);
			return ;
		}
		//test_static_add(trie,ptr_table,words_in,FOUND);
	}
	//printf ("free\n");
	printf("SUCCED:everything is ok in static_trie\n");
	free(line);
	cleanup(ptr_table);
	fclose(fd);


	return ;
}



