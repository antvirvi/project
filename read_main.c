#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cleanup_A(char ** ptr,int A_table_size){
	int a;
	for(a=0;a<A_table_size;a++){
		free(ptr[a]);
	}
	free(ptr);
}

int test_input(char * filename)
{

	int words_in = 0;
	int A_words_in = 0;
	int A_word_size=512;
	int word_size=512;
	int A_table_size=2048;
	int table_size=2048;
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
	int count=0;
	int previous_is_Q=1;
	int current_version=0;
	int k;
	int delete_batch=0;	
	int word_len;

	int length_array_capacity=10;
	int last_word=0;
	int lengths_taken=0;

	int *Q_lengths=malloc(length_array_capacity*sizeof(int));
	int *version=malloc(length_array_capacity*sizeof(int));
	int *start=malloc(length_array_capacity*sizeof(int));

	char **ptr_table = malloc(table_size*sizeof(char *));
	int *word_lengths=malloc(table_size*sizeof(int));


	char **A_ptr_table = malloc(A_table_size*sizeof(char *));


	for(a=0;a<table_size;a++){
		A_ptr_table[a]=malloc(A_word_size*sizeof(char));
		ptr_table[a]=malloc(word_size*sizeof(char));
		word_lengths[a]=word_size;
		}
	words_in = 0;
	while ((read = getline(&line, &len, fd)) != -1) {

		//words_in = 0;
		
		word = strtok (line," \n");

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
				words_in=0;
				word=strtok(NULL,"\n");
				int k;
				//print_hash_version(trie->hash);
				//execute_queries(trie->hash,ptr_table,ptr_lengths,version,start,lengths_taken,top);
				lengths_taken=0;
				last_word=0;
				count++;				
				if(word!=NULL){
					count++;
					k=atoi(word);
					//printf("count is %d",count);
					//if(count==1) break; 		
				}
				//if(count==1) break; 		
				delete_batch=0;
				continue;
			}
			else{
				continue;
			}
			word=strtok(NULL," \n");
				
			if(flag==1){
				//printf("here\n");
				while(word!=NULL){

					if(words_in==table_size-1){
						//printf("Doubling ptr_table\n");
						ptr_table = realloc(ptr_table,table_size*2*sizeof(char*));
						word_lengths = realloc(word_lengths,table_size*2*sizeof(int));
						if(ptr_table==NULL || word_lengths==NULL) exit(-1);
						for(a=table_size;a<(table_size*2);a++){
							ptr_table[a]=malloc(word_size*sizeof(char));
							if(ptr_table[a]==NULL) exit(-1);
							word_lengths[a] =word_size;
							}
						
				//printf("word_size here %d\n",word_size);
						table_size*=2;
					}
					word_len=strlen(word);
					if(word_len>=word_lengths[words_in]){
						printf("doubling\n");
						while(word_len>=word_lengths[words_in]){
							//printf("word_size is %d\n",word_size);
							word_lengths[words_in]*=2;
							}
							ptr_table[words_in] = realloc(ptr_table[words_in],word_lengths[words_in]*sizeof(char));
						}	
						strcpy(ptr_table[words_in],word);
						//word_lengths[words_in]=word_len;	
					printf("I wrote in pos %d the string %s\n",words_in,ptr_table[words_in]);			
						words_in++;				
						word=strtok(NULL," \n");
					
				}
					}
				else{
					A_words_in=0;
					while(word!=NULL){
						if(A_words_in==A_table_size-1){
					//table_size*=2;
							A_ptr_table = realloc(A_ptr_table,A_table_size*2*sizeof(char*));
							if(A_ptr_table==NULL) exit(-1);
							for(a=A_table_size;a<(A_table_size*2);a++){
								A_ptr_table[a]=malloc(A_word_size*sizeof(char));
								if(A_ptr_table[a]==NULL) exit(-1);
								}
				//printf("word_size here %d\n",word_size);
							A_table_size*=2;
						}
						while(strlen(word)>=A_word_size){
							A_word_size=A_word_size*2;
							for(a=0;a<A_table_size;a++){
								A_ptr_table[a] = realloc(A_ptr_table[a],A_word_size*sizeof(char));
								if(A_ptr_table[a]==NULL) exit(-1);
								}
						}
						//printf("writing in table pos %d : %s\n",A_words_in,word);
						strcpy(A_ptr_table[A_words_in],word);	
						A_words_in++;				
						word=strtok(NULL," \n");
						}
					}	

		switch(flag){
			case 1 :
				if(lengths_taken==length_array_capacity){
					length_array_capacity*=2;

					Q_lengths=realloc(Q_lengths,length_array_capacity*sizeof(int));
					start=realloc(start,length_array_capacity*sizeof(int));
					version=realloc(version,length_array_capacity*sizeof(int));					
				}
				//printf("i passed %d , start %d ,words in %d\n ",words_in-last_word,last_word,words_in);
				Q_lengths[lengths_taken]=words_in-last_word;
				start[lengths_taken]=last_word;
				version[lengths_taken]=current_version;

				last_word=words_in;
				lengths_taken++;
				//this
				//command_error=lookupTrieNode_with_bloom_versioning(trie->hash,ptr_table,words_in-1,top,current_version); //kfrm
								
				previous_is_Q=1;
				break;
			case 2 :

				if(previous_is_Q==1){
					//printf("changed version\n");
					current_version++;}
				previous_is_Q=0;
	
				A_words_in=0;			
				break;
			case 3 :

				delete_batch++;
				//printf("deleting word \"%s \" in version %d \n",ptr_table[0],current_version);
				//command_error=deleteTrieNode_versioning(trie->hash,ptr_table,words_in,current_version);
				if(previous_is_Q==1){
					//printf("changed version\n");
					current_version++;}

				previous_is_Q=0;

				A_words_in=0;			
				break;
		
		}
		flag=0;	
}


  	free(line);

	free(Q_lengths);
	free(start);
	free(version);


	cleanup_A(A_ptr_table,A_table_size);
	cleanup_A(ptr_table,table_size);
	fclose(fd);
	
return 0;

}




int main (int argc, char **argv ){	
//	clock_t begin = clock();
	int c;
	char *init_file;
	char query_file[50];
	int init_result;
	
	
	//strcpy(query_file,"small.work");
	strcpy(query_file,"medium_dynamic.work");

	if(test_input(query_file)<0) return -1;
	


	return 0;	
}
