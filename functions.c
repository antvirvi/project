#ifndef FUNC_H
#define FUNC_H
#include "functions.h"
 
//#include "bloomfilter.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

//Second Part of Project

//#define ClearBit(A,k)   ( A[(k/sizeof(int))] &= ~(1 << (k%sizeof(int))) )
//#define TestBit(A,k)    ( A[(k/sizeof(int))] & (1 << (k%sizeof(int))) ) 
//#define SetBit(A,k)     ( A[(k/sizeof(int))] |= (1 << (k%sizeof(int))) )

#endif

#include "libraries.h"
//
#include <stdio.h>
//
/*extern*/ int buffer_size = 16;
/*extern*/ int word_size = 8;
/*extern*/ int table_size = 4;
int threads_quantity  = 12 ;

void printtable(char **pt, int num){
	int a;
	for(a=0;a<=num;a++){
		printf("\x1b[31m""#Table %d: %s %ld\n""\x1b[0m",num ,pt[a],strlen(pt[a]));
	}
}

int init_input(struct index *trie,char * filename){
	//printf("\x1b[32m""INIT_INPUT start\n""\x1b[0m");
	int a;
	char **ptr_table = malloc(table_size*sizeof(char *));
	int words_in = 0;

	FILE* fd = fopen(filename, "r"); //opening input file
	//strcpy(buffer,"\0");
		
	if(fd == NULL){
		perror("Error opening input file");
		printf("File name %s\n",filename);
		return -1;
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *word;
	int static_flag=0;

	for(a=0;a<table_size;a++)
		ptr_table[a]=malloc(word_size*sizeof(char));
			
	//read first word
	if((read = getline(&line, &len, fd)) != -1){
		words_in = 0;
		word = strtok (line," \n");
		if(strcmp(word,"STATIC")==0) static_flag=1;
	}

	rewind(fd);
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
				for(a=words_in;a<table_size;a++) //a=0
					ptr_table[a] = realloc(ptr_table[a],(word_size+1)*sizeof(char));
			}
		//	ptr_table[words_in] = malloc(word_size*sizeof(char));
			strcpy(ptr_table[words_in],word);
			words_in++;
			word=strtok(NULL," \n");
		}
		//append_trie_node_iterative(trie->root,ptr_table,0,words_in-1);
		//append_trie_node(trie->root,ptr_table,0,words_in-1);
		insertTrieNode(trie->hash,ptr_table,words_in);
	}
	free(line);
	cleanup(ptr_table);
	fclose(fd);
	return static_flag;	
}




int test_input(struct index *trie,char * filename)
{

	int words_in = 0;
	int flag; //1 question, 2 addition, 3 deletion, 4 end of file
	int a;
	FILE* fd = fopen(filename, "r"); //opening input file

	if(fd == NULL)
	{
		perror("Error opening input file");
		return -1;
	}

	topk *top;
	top=create_top(top);
	top=init_top(top);

	JobScheduler *JS = initialize_scheduler(threads_quantity);
	printf("Initial nado %p\n",&JS->q);

/* TEST FOR JOBS
	initialize_scheduler(threads_quantity);
	Job * job_pool = malloc(8*sizeof(Job));
	job_pool[0].text = malloc(strlen("Margarita0"));
strcpy(job_pool[0].text,"Margarit0");
	job_pool[1].text = malloc(strlen("Margarita1"));
strcpy(job_pool[1].text,"Margarit1");
	job_pool[2].text = malloc(strlen("Margarita2"));
strcpy(job_pool[2].text,"Margarit2");
	job_pool[0].opt=pr;
	job_pool[1].opt=pr;
	job_pool[2].opt=pr;
	job_pool[0].opt(job_pool[0].text);
	job_pool[1].opt(job_pool[1].text);
	job_pool[2].opt(job_pool[2].text);

*/
//return ; //should be removed
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *word;
	int command_error;
	int count=0;
	char **ptr_table = malloc(table_size*sizeof(char *));
	Job * j;
//	char * ngram;
	//paths *paths_=init_paths(4,10); 

	for(a=0;a<table_size;a++)
		ptr_table[a]=malloc(word_size*sizeof(char));

	while ((read = getline(&line, &len, fd)) != -1) {
		//words_in = 1;
		words_in = 0;
		
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

				word=strtok(NULL,"\n");
				int k;
				print_print(top);
				if(word!=NULL){
					count++;
					k=atoi(word);
					//printf("count is %d",count);
					//Sprint_top(top,k);
					execute_all_jobs(JS);
					//if(count==42) break; 		
				}
				top=init_top(top);
			}
			else if(strcmp(word,"\0")==0){ 
				continue;
			}
			word=strtok(NULL," \n");
				
			while(word!=NULL){
			if(words_in==table_size-1){
				//table_size*=2;
				ptr_table = realloc(ptr_table,table_size*2*sizeof(char*));
				if(ptr_table==NULL) exit(-1);
				for(a=table_size;a<(table_size*2);a++){
					ptr_table[a]=malloc(word_size*sizeof(char));
					if(ptr_table[a]==NULL) exit(-1);
					}
				//printf("word_size here %d\n",word_size);
				table_size*=2;
			}
			while(strlen(word)>=word_size){
				word_size=word_size*2;
				for(a=0;a<table_size;a++){
					ptr_table[a] = realloc(ptr_table[a],word_size*sizeof(char));
					if(ptr_table[a]==NULL) exit(-1);
					}
			}
			/**if(strlen(word)>=word_size){ 
				while(strlen(word)>=word_size){ word_size=word_size*2;}		
				//for(a=0;a<table_size;a++){
				ptr_table[words_in] = realloc(ptr_table[words_in],word_size*sizeof(char));
						if(ptr_table[words_in]==NULL) exit(-1);
					//	}
			}*/
				//ptr_table[words_in] = malloc(word_size*sizeof(char));
		strcpy(ptr_table[words_in],word);
				
		words_in++;				
			
		word=strtok(NULL," \n");
		}

		switch(flag){
			case 1 :
				//command_error=search_in_trie(trie->root,ptr_table,words_in-1);
				j = malloc(sizeof(Job));
//				j->opt=lookupTrieNode_with_bloom;
//				j->opt = get_a_job;
				j->opt = pr;
				j->hash =trie->hash;
				j->words = ptr_table;
				j-> number_of_words = words_in-1;
				j->top = top;
//				j->opt(j);
//				j->opt();
		//		j->opt(j->hash,j->words,j-> number_of_words,j->top);
				if(!pthread_mutex_lock(&R)){
					pthread_cond_wait(&tcv,&T);
				submit_job(JS,&j);
			
//				command_error=lookupTrieNode_with_bloom(trie->hash,ptr_table,words_in-1,top); 
	//			if(command_error==-1) printf("%d\n",command_error);
				break;
			case 2 :
				//command_error=append_trie_node_iterative(trie->root,ptr_table,0,words_in-1);
				//command_error=append_trie_node(trie->root,ptr_table,0,words_in-1);
				command_error=insertTrieNode(trie->hash,ptr_table,words_in);
				break;
			case 3 :
				//command_error=delete_ngram(trie->root,ptr_table,0,words_in-1);
				command_error=deleteTrieNode(trie->hash,ptr_table,words_in);
				//printf("delete error is word (%s) is %d \n",ptr_table[0],command_error);
				break;
		
		}
		flag=0;	
}
  	free(line);
	erase_top(top);
	cleanup(ptr_table);
	fclose(fd);
	
return 0;

}

void cleanup(char ** ptr){
	int a;
	for(a=0;a<table_size;a++){
		free(ptr[a]);
	}
	free(ptr);
}	

void print_node(trie_node *node){
	printf("node word is %s\n",node->word);
	printf("node has %d children\n",node->number_of_childs);
	printf("node is final: %c\n",node->is_final);
}

void print_trie(trie_node *node,int level){
	int i;
	if(node->number_of_childs==0){printf("\n"); return;}
	//printf("In level %d : \n",level );
	for(i=0;i<node->number_of_childs;i++){
		//print_node(&(node->children[i]));
		if(level!=0) printf("->");
		printf("%s (%c) ",node->children[i].word,node->children[i].is_final);
		print_trie(&(node->children[i]),level+1);
	}
	return;
}

trie_node *init_trie(){
	trie_node *root=malloc(sizeof(trie_node));
	root->word=NULL;
	root->is_final='n';
	root->number_of_childs=0;	
	root->max_childs=MAX_CHILDS;
	
	root->children=malloc(CHILD_NUM*sizeof(struct trie_node));
	//for(i=0;i<CHILD_NUM;i++) root->children[i]=NULL;

	return root;
}

void destroy_childs(trie_node *node){
	int i;
	for(i=0;i<node->number_of_childs;i++){
		destroy_childs(&(node->children[i]));
		free(node->children[i].word);	
	}
	free(node->children);
}
void delete_trie(struct index *trie){
	destroy_childs(trie->root);
	free(trie->root);
	free(trie);

}

trie_node *create_trie_node(char *word,char is_final){
	trie_node *node=malloc(sizeof(trie_node));
	if(strlen(word)+2>WORD_SIZE){
		node->word=malloc((strlen(word)+2)*sizeof(char));
		strcpy(node->word,word);
		memset(node->static_word,0,WORD_SIZE);
	}
	else{
		strcpy(node->static_word,word);
		node->word=node->static_word;
	}
	node->is_final=is_final;
	node->number_of_childs=0;
	node->max_childs=MAX_CHILDS;


	node->children=malloc(CHILD_NUM*sizeof(struct trie_node));

	return node;
}

trie_node *init_trie_node(trie_node *node,char *word,char is_final){
	
	
	node->word=malloc(WORD_SIZE*sizeof(char));
	strcpy(node->word,word);
	
	node->is_final=is_final;
	node->number_of_childs=0;
	node->max_childs=MAX_CHILDS;

	node->children=malloc(CHILD_NUM*sizeof(struct trie_node));
	

	return node;
}

int append_trie_node(trie_node *root,char **word,int word_number,int number_of_words){
	int error;
	if(word_number>number_of_words){
		//printf("out of words to add\n");
		return SUCCESS;
		}
	//printf("append trie node %d out of %d word: %s\n",word_number,number_of_words,word[word_number]);
	char is_final='n';
	if(word_number==number_of_words) is_final='y';

	if(root->number_of_childs==0){
		error=append_word(root,0,word[word_number],is_final);
		if(error==ERROR) return ERROR;
		root->number_of_childs++;
		append_trie_node(&(root->children[0]),word,word_number+1,number_of_words);
	}
	else{
		int pos;
		int exists=check_exists_in_children(root,word[word_number],&pos);
		if (exists==1){
			//make the node also final
			if((&(root->children[pos]))->is_final!='y') (&(root->children[pos]))->is_final=is_final; //this change
			append_trie_node(&(root->children[pos]),word,word_number+1,number_of_words);
			}
		else{	
			error=append_word(root,pos,word[word_number],is_final);
			if(error==ERROR) return ERROR;
			root->number_of_childs++;
			append_trie_node(&(root->children[pos]),word,word_number+1,number_of_words);
			}
	}
	return SUCCESS; //0 errors
}

int append_trie_node_iterative(trie_node *root,char **word,int word_number,int number_of_words){
	int error;
	char is_final;
	trie_node *node=root;
	int pos,exists;
	for(word_number=0;word_number<=number_of_words;word_number++){
		is_final='n';
		if(word_number==number_of_words) is_final='y';
		
		if(node->number_of_childs==0){
			error=append_word(node,0,word[word_number],is_final);
			if(error==ERROR) return ERROR;
			node->number_of_childs++;
			node=&(node->children[0]);
		}
		else{
			exists=check_exists_in_children(node,word[word_number],&pos);
			if (exists==1){
				if((&(node->children[pos]))->is_final!='y') (&(node->children[pos]))->is_final=is_final; //this change
				node=&(node->children[pos]);
				}
			else{	
				error=append_word(node,pos,word[word_number],is_final);
				if(error==ERROR) return ERROR;
				node->number_of_childs++;
				node=&(node->children[pos]);
				}
			}
				
	}
	if(word_number>number_of_words){
		//printf("out of words to add\n");
		return SUCCESS;
		}


	
	return SUCCESS; //0 errors
}


int check_exists_in_children(trie_node *node,char *word,int *pos){
		int pivot=0; //pivor is integer so in the division it will rounf in the smaller absolute value 5/2=2
		int lower=0;
		int upper=node->number_of_childs-1;
		int compare;
		//printf("inside check exists\n");
		//if(upper==-1) return 0; //i made this change
		while(1!=0){
			//printf("upper %d lower %d pivot %d\n",upper,lower,pivot);
			if(upper<=lower){
				pivot=(upper+lower)/2;
				compare=strcmp(node->children[pivot].word,word);
				if(compare==0){
					*pos=pivot;
					return 1; //exact match
				}
				//printf("compare is %d\n",compare);	
				*pos=(compare<0)? pivot+1:pivot; //lower+1:lower
				return 0; //not exact match
				}
			else {
				pivot=(upper+lower)/2;
				compare=strcmp(node->children[pivot].word,word); // equal=0 children[i]<word: compare<0 children>word : compare>0
				if(compare==0) {
					*pos=pivot;
					return 1; //exact match
				}
				else if(compare>0) upper=pivot-1;
				else lower=pivot+1;
			}
		}
}

int append_word(trie_node *node,int pos,char *word,char is_final){
		//printf("inside append_word , pos %d\n",pos);
		if(node->number_of_childs==node->max_childs){
				//printf("I have to double the children\n");
				node->children=realloc(node->children,node->max_childs*2*sizeof(trie_node));
				//printf("Done Realloc \n");
				if(node->children==NULL) return ERROR;
		        node->max_childs*=2;
				//print_node(&(node->children[0]));	
		}

		trie_node * backup=node->children;
		memmove(node->children,backup,pos*sizeof(trie_node));
		memmove(node->children+(pos+1),backup+(pos),(node->number_of_childs-pos)*sizeof(trie_node));
		init_trie_node(&(node->children[pos]),word,is_final);
		return SUCCESS ; //zero errors
}

int delete_ngram(trie_node *root,char **word,int word_number,int number_of_words){
		int error;
		//printf("in delete ngram word \"%s\"\n",word[word_number]);
		if(word_number==number_of_words+1){
			if(root->number_of_childs!=0 && root->is_final!='y') return ERROR;
			if(root->number_of_childs!=0 &&root->is_final=='y'){
				root->is_final='n';
				return 2;}  
			return SUCCESS;//and return no error . the previous one is gonna delete it	
		}
		if(root->number_of_childs==0) return ERROR;
		else{
			int pos;
			//printf("before exists \"%s\"\n",word[word_number]);
			int exists=check_exists_in_children(root,word[word_number],&pos);
			//printf("exists :%d ",exists);
			if (exists==1){
				error=delete_ngram(&(root->children[pos]),word,word_number+1,number_of_words);
				if(error==0)
				{
					if(root->children[pos].is_final=='y' && word_number!=number_of_words ) return 2; //return 2 if  i am del half 
					error=delete_from_node(root,pos);
					root->number_of_childs--;
				}
				if(error!=1 && root->number_of_childs!=0) return 2; //dont delete the node if it has more childs 
				return error; 
			}
			else return ERROR;	//return error if the word is not on the trie , so the ngram is not in the trie
			}
}



int delete_from_node(trie_node *node,int pos){
		//printf("in delete node pos %d word to delete is %s\n",pos,node->children[pos].word);

		trie_node * backup=node->children;
		trie_node *node_to_delete=&(node->children[pos]);
		destroy_childs(node_to_delete);
		free(node_to_delete->word);
		memmove(node->children,backup,pos*sizeof(trie_node));
		memmove(node->children+pos,backup+pos+1,(node->number_of_childs-(pos+1))*sizeof(trie_node));
		//printf("out of delete\n");
		//free(node_to_delete->children);
		return SUCCESS ; //zero e-rrors
}

int search_in_trie_without_blfilter(trie_node *root,char **word,int number_of_words){
	//printf("Inside search\n");
	stack *stack_=init_stack();
	int word_number;
	int exists;
	int pos;
	trie_node *node;
	int start=0;
	paths *paths_=init_paths(4,10); //rows columns
	while(start!=number_of_words+1) {
		word_number=start;
		node=root;
		while(node->number_of_childs!=0) {
			//printf("word number :%d %s\n",word_number,word[word_number]);
			if(node->is_final=='y') {
				//print_nodes_from_stack(root,stack_);
				check_in_paths3(paths_,stack_,root); //I found it
				}
			exists=check_exists_in_children(node,word[word_number],&pos);
			if(exists==0) break;
			//printf("I am gonna push : %d\n",pos);
			push(stack_,pos);
			node=&(node->children[pos]);
			word_number++;
		}
		if(exists==1) {
			check_in_paths3(paths_,stack_,root);
			//print_nodes_from_stack(root,stack_);
		}
		reset_stack(stack_);
		start++;
	}
	printf("\n");
	int found=SUCCESS;
	if(paths_->words_in==0) found=-1;
	//print_paths(paths_);
	stack_destroy(stack_);
	delete_paths(paths_); //rows	
	return found;
	if(exists==0) return ERROR;
	
	return SUCCESS;	


}



void print_nodes_from_stack(trie_node *root,stack *stack_){
	int number=get_stack_number(stack_);
	int i ,pos;
	trie_node *node=root;
	//printf("Found N gram: ");
	for(i=0;i<number-1;i++){
		pos=get_stack_elements(stack_,i);
		node=&(node->children[pos]);
		printf("%s ",node->word);
	}
	pos=get_stack_elements(stack_,i);
	node=&(node->children[pos]);
	printf("%s",node->word);
	printf(YELLOW"|"RESET);
}

void print_nodes_from_hash(hash_layer *hash,stack *stack_){
	int number=get_stack_number(stack_);
	int i ,pos;
	trie_node *node;
	//printf("Found N gram: ");
	pos=get_stack_elements(stack_,0);
	hash_bucket *bucket=&(hash->buckets[pos]);
	pos=get_stack_elements(stack_,1);
	node=&(bucket->children[pos]);
	if(number==2){
		printf("%s|",node->word);
		return;
	}
	printf("%s ",node->word); 
	for(i=2;i<number-1;i++){
		pos=get_stack_elements(stack_,i);
		node=&(node->children[pos]);
		printf("%s ",node->word);
	}
	
	pos=get_stack_elements(stack_,i);
	node=&(node->children[pos]);
	printf("%s",node->word);
	printf("|");
}

paths *init_paths(int rows,int columns){
	int i;
	paths* paths_=malloc(sizeof(paths));

	paths_->paths_array=malloc(rows*sizeof(int*));
	if(paths_->paths_array==NULL) return NULL;
	
	for(i=0;i<rows;i++) {
		paths_->paths_array[i]=malloc(PATH_COLUMN*sizeof(int));
		if(paths_->paths_array[i]==NULL) return NULL;
	}	
	paths_->max_words=rows;
	paths_->words_in=0;
	return paths_;
} 

int check_in_paths3(paths *paths_, stack *stack_,trie_node *root){//initialize paths in -1
	//printf("inside check in paths\n");
	int number=get_stack_number(stack_);
	int found=0;
	int pos,i,j;
	for(i=0;i<paths_->words_in;i++){
		for(j=0;j<number;j++){
			pos=get_stack_elements(stack_,j);
			if(paths_->paths_array[i][j]!=pos){
				break;
			} //if it finds it continue with the next word
		}
		if(j==number) {found=1; break;} //if it doesnt find one node f the path then leave
 			
	}
	if(found==0) {
		add_to_paths(paths_,stack_); //add to paths and print it
		//print_nodes_from_stack2(root,stack_);
		print_nodes_from_stack(root,stack_);
	}
	return found;
}

int check_in_paths4(paths *paths_, stack *stack_,hash_layer *hash){//trie_node *root){//initialize paths in -1
	//printf("inside check in paths\n");
	int number=get_stack_number(stack_);
	int found=0;
	int pos,i,j;
	for(i=0;i<paths_->words_in;i++){
		for(j=0;j<number;j++){
			pos=get_stack_elements(stack_,j);
			if(paths_->paths_array[i][j]!=pos){
				break;
			} //if it finds it continue with the next word
		}
		if(j==number) {found=1; break;} //if it doesnt find one node f the path then leave
 			
	}
	if(found==0) {
		add_to_paths(paths_,stack_); //add to paths and print it
		print_nodes_from_hash(hash,stack_);
	}
	return found;
}

void add_to_paths(paths *paths_, stack *stack_){
	int path_num=paths_->words_in;
	if(path_num==paths_->max_words) double_paths(paths_);
	int number=get_stack_number(stack_);
	int i ,pos;
	//printf("Found N gram: ");
	for(i=0;i<number;i++){
		pos=get_stack_elements(stack_,i);
		paths_->paths_array[path_num][i]=pos;
	}
	for(i=number;i<PATH_COLUMN; i++) paths_->paths_array[path_num][i]=-1;
	//printf("|");
	paths_->words_in++;

}

int double_paths(paths *paths_){
	//printf("In double paths\n");
	//int **temp;
	int i;
	paths_->paths_array=realloc(paths_->paths_array,2*paths_->max_words*sizeof(int*));
	if(paths_->paths_array==NULL) return ERROR;
	for(i=paths_->max_words;i<2*paths_->max_words;i++){
		paths_->paths_array[i]=malloc(PATH_COLUMN*sizeof(int));
		if(paths_->paths_array[i]==NULL) return ERROR;	
	}
	paths_->max_words*=2;
	return SUCCESS;
}

void delete_paths(paths *paths_){
	int i;
	for(i=0;i<paths_->max_words;i++){
		//printf("freed\n");
		free(paths_->paths_array[i]);
	}
	free(paths_->paths_array);
	free(paths_);
}

void print_paths(paths *paths_){
	printf("\nin print paths\n");
	printf("words in are %d\n",paths_->words_in);
	int i;
	int j;
	for(i=0;i<paths_->words_in;i++){
		for(j=0;j<3;j++){
		printf("%d -> ",paths_->paths_array[i][j]);
		}
		printf("\n");
	}
}


char * myappend(char * string, char * word){
	if(string==NULL) 
		string = malloc(0);
	size_t new_length = strlen(string)+strlen(word)+2;
	char *string2;
	string2 = malloc(new_length);
	strcpy(string2,string);
	strcat(string2," ");
	strcat(string2,word);
//	string =  realloc(string,new_length);
//	strcpy(string,string2);
	free(string);

	if(string2[0] == ' ') //lets chop off the first space in each n gram
	{
		int i;
		for(i=0;i<strlen(string2);i++)
			string2[i]=string2[i+1];
	}
	return string2;

}

void  myappend_pan(char **string,int *str_size, char * word){

	int string_len=strlen(*string);
	int word_len=strlen(word);

	if((*str_size)<=string_len+word_len+1){
		//*str_size=*str_size*2*((string_len+word_len+1)/ *str_size);
		(*str_size)=(*str_size)*2;
		while(*str_size<=string_len+word_len+1) *str_size=(*str_size)*2;

		*string=realloc(*string, (*str_size)*sizeof(char));
	}
	strcat(*string,word);
	
	//to avoid str_cat
	//memcpy(*string,word,(word_len+1)*sizeof(char));
	//(*string)[string_len+word_len]='\0';

}

void  myappend_pan_with_space(char **string,int *str_size, char * word){
	
	int string_len=strlen(*string);
	int word_len=strlen(word);

	if((*str_size)<=string_len+word_len+1){

		*str_size=*str_size*2*((string_len+word_len+1)/ *str_size);
		*string=realloc(*string, (*str_size)*sizeof(char));
	}

	strcat(*string," ");
	strcat(*string,word);
	
	//To avoid strcat
	//memcpy(&((*string)[string_len+1]),word,(word_len)*sizeof(char));
  	//(*string)[string_len+word_len+1]='\0';



}

void initialize_bucket(hash_bucket *bucket,int m){
	bucket->children=malloc(m*sizeof(trie_node));
	bucket->children_number=0;
	bucket->capacity=m;
}

hash_layer *createLinearHash(int c ,int m){ //c is number of buckets ,m is number of cells per bucket
	int i;
	hash_layer *hash=malloc(sizeof(hash_layer));
	if(hash==NULL) return NULL;

	hash->bucket_capacity=m;
	hash->buckets_number=c;
	hash->load_factor=0.9;
	hash->total_children=0;
	hash->bucket_to_split=0;
	hash->split_round=0;
	hash->buckets=malloc(c*sizeof(hash_bucket));
	if(hash->buckets==NULL) return NULL;
	
	for(i=0;i<c;i++){
		initialize_bucket(&(hash->buckets[i]),m);
	}
	return hash;
}


int  hash_function(hash_layer *hash, char *word)
{	
	int hash_value;
    unsigned long hash_int = 5381;
    int c;

    while (c = *word++)
        hash_int = ((hash_int << 5) + hash_int) + c; /* hash * 33 + c */
	//printf("hash is %ld\n",hash_int);
	
	hash_value=hash_int%(C<<hash->split_round);
	//hash_value=hash_int%(C*(int)pow(2,hash->split_round));

	int temp=hash_int%(C<<(hash->split_round+1));
	//int temp=hash_int%(C*(int)pow(2,hash->split_round+1));
	if(temp<hash->buckets_number) return temp;
	//printf("new hash value after is %ld\n",hash_value);
    return hash_value;
}


void destroyLinearHash(hash_layer *hash){
	int i;
	hash_bucket *bucket;
	for(i=0;i<hash->buckets_number;i++){
		bucket=&(hash->buckets[i]);
		//printf("bucket %d\n",i);
		destroy_bucket_nodes(bucket);
		//destroy_buckets(bucket,0);
		//free(bucket->children);
	}
	free(hash->buckets);
	free(hash);
	return ;
}

void destroy_bucket_nodes(hash_bucket *bucket){
	trie_node *node;
	int j;
	for(j=0;j<bucket->children_number;j++){
		node=&(bucket->children[j]);
		destroy_childs(node);
		free(node->word);
		//free(node);
		}
	free(bucket->children);
}

int insertTrieNode(hash_layer *hash,char **words,int word_number){
	//printf("appending %s\n",words[0]);
	char is_final='n';
	if(word_number==1) is_final='y';

	//int hash_val=hash_function(hash,words[0]);

	if((hash->total_children/((float)hash->buckets_number*hash->bucket_capacity)) > hash->load_factor){
		//printf("total children are %d, buckets_number are %d\n",hash->total_children,hash->buckets_number);
        //printf("load factor: %f \n",(hash->total_children/((float)hash->buckets_number*hash->bucket_capacity)));
		//printf("capacity: %d \n",hash->bucket_capacity);
		int resize_error=resize_hash(hash);
		if(resize_error==ERROR) return ERROR;
	}
	
	int hash_val=hash_function(hash,words[0]);
	

	trie_node *node;
	node=add_to_backet(hash,hash_val,words[0],is_final); //node gets the value of the first trie node

	if(word_number>1){ 
		append_trie_node(node,words,1,word_number-1);
		//node->number_of_childs++;
		return 0;	
	}	
	
	if(node->is_final=='n') node->is_final=is_final;
	return 0;
}


/*int check_exists_in_bucket(hash_bucket *bucket,char *word,int *pos){
		int pivot=0; //pivor is integer so in the division it will rounf in the smaller absolute value 5/2=2
		int lower=0;
		if(bucket->children_number==0) {
			*pos=0;
			return 0;
		}
		int upper=bucket->children_number-1;
		//printf("inside check exists\n");
		//if(upper==-1) return 0; //i made this change
		while(1!=0){
			//printf("upper %d lower %d pivot %d\n",upper,lower,pivot);
			if(upper<=lower){
				pivot=(upper+lower)/2;
				int compare=strcmp(bucket->children[pivot].word,word);
				if(compare==0){
					*pos=pivot;
					return 1; //exact match
				}
				//printf("compare is %d\n",compare);	
				*pos=(compare<0)? pivot+1:pivot; //lower+1:lower
				return 0; //not exact match
				}
			else {
				pivot=(upper+lower)/2;
				int compare=strcmp(bucket->children[pivot].word,word); // equal=0 children[i]<word: compare<0 children>word : compare>0
				if(compare==0) {
					*pos=pivot;
					return 1; //exact match
				}
				else if(compare>0) upper=pivot-1;
				else lower=pivot+1;
			}
		}
}*/

int check_exists_in_bucket(char *word,int *pos,trie_node *children,int children_number){
		int pivot=0; //pivor is integer so in the division it will rounf in the smaller absolute value 5/2=2
		int lower=0;
		int compare;
		if(children_number==0) {
			*pos=0;
			return 0;
		}
		int upper=children_number-1;
		//if(upper==-1) return 0; //i made this change
		while(1!=0){
			//printf("upper %d lower %d pivot %d\n",upper,lower,pivot);
			if(upper<=lower){
				pivot=(upper+lower)/2;
				compare=strcmp(children[pivot].word,word);
				if(compare==0){
					*pos=pivot;
					return 1; //exact match
				}
				//printf("compare is %d\n",compare);	
				*pos=(compare<0)? pivot+1:pivot; //lower+1:lower
				return 0; //not exact match
				}
			else {
				pivot=(upper+lower)/2;
				compare=strcmp(children[pivot].word,word); // equal=0 children[i]<word: compare<0 children>word : compare>0
				if(compare==0) {
					*pos=pivot;
					return 1; //exact match
				}
				else if(compare>0) upper=pivot-1;
				else lower=pivot+1;
			}
		}
}



trie_node* add_to_backet(hash_layer *hash,int hash_val,char *word,char is_final){
	//printf("In add to bucket \"%s\"\n",word);
	
	
	hash_bucket *bucket=&(hash->buckets[hash_val]);
	
	int *last=&(bucket->children_number);
	trie_node *node;


	if(*last==bucket->capacity){ //initializing overflow bucket
		bucket->children=realloc(bucket->children,2*bucket->children_number*sizeof(trie_node));
		if(bucket->children==NULL) return NULL;
		bucket->capacity=bucket->capacity*2;
		//printf("I made an overflow at bucket %d\n",hash_val);
	} 

	int pos;
	//int exists=check_exists_in_bucket(bucket,word,&pos);
	int exists=check_exists_in_bucket(word,&pos,bucket->children,bucket->children_number);
	node=&(bucket->children[pos]);
	if(exists==1) return node;
	//memove nodes to the right
	trie_node *backup=bucket->children;
	memmove(bucket->children,backup,pos*sizeof(trie_node));
	memmove(bucket->children+(pos+1),backup+(pos),(bucket->children_number-pos)*sizeof(trie_node));//
	init_trie_node(node,word,is_final);

	*last=*last+1;
	hash->total_children++;
	//int resize_error;
	//printf("children now %d\n",*last);
	return node;
}

int resize_hash(hash_layer *hash){
	hash_bucket *bucket;
	//printf("attempting to expand buckets\n");
	hash->buckets=realloc(hash->buckets,(hash->buckets_number+1)*sizeof(hash_bucket)); //add bucket lineat
	if(hash->buckets==NULL){
		printf("error in realloc\n");
		return ERROR;
	}
	hash->buckets_number++;
	//initialize bucket
	initialize_bucket(&(hash->buckets[hash->buckets_number-1]),hash->bucket_capacity);
	int i;
	hash_bucket *new_bucket=&(hash->buckets[hash->buckets_number-1]); //pointer to the new bucket
	bucket=&(hash->buckets[hash->bucket_to_split]); //re arranging bucket to split
	int new_hash_val=-1;
	stack *stack_=init_stack();
	//print_stack(stack_);

	if(bucket->children_number==0){
		hash->bucket_to_split=(hash->bucket_to_split+1)%(C<<hash->split_round);//without pow
		//hash->bucket_to_split=(hash->bucket_to_split+1)%(C*(int)pow(2,hash->split_round));//
		if(hash->bucket_to_split==0 && hash->buckets_number>C) hash->split_round++;
		stack_destroy(stack_); 
		return SUCCESS;
	} // no need for rearranging bucket
	
	//printf("Bucket to split is %d and split round %d in bucket %d\n",hash->bucket_to_split,hash->split_round,hash->buckets_number-1);
	//printf("children in bucket is %d\n",bucket->children_number);
	int previous=hash->bucket_to_split;
	hash->bucket_to_split=(hash->bucket_to_split+1)%(C<<hash->split_round);// without pow
 	//hash->bucket_to_split=(hash->bucket_to_split+1)%(C*(int)pow(2,hash->split_round));//
	for(i=0;i<bucket->children_number;i++){

		new_hash_val=hash_function(hash,bucket->children[i].word);
		//printf("new hash val is %d \n",new_hash_val);
		if(new_hash_val==previous) continue; //if hash value is the same then no need to change bucket
		if(new_bucket->children_number==new_bucket->capacity){	//if new bucket fills then create an overflow bucket
			new_bucket->children=realloc(new_bucket->children,bucket->capacity*2*sizeof(trie_node));
			if(new_bucket->children==NULL){
				stack_destroy(stack_); 
				return ERROR;
			}
			new_bucket->capacity=new_bucket->capacity*2;	
		}
		memmove(&(new_bucket->children[new_bucket->children_number]),&(bucket->children[i]),sizeof(trie_node)); //copy nod
		push(stack_,i);		
		new_bucket->children_number++;	
	}

	//print_stack(stack_);
	shrink_buckets(&(hash->buckets[previous]),stack_);
	if(hash->bucket_to_split==0 && hash->buckets_number>C) hash->split_round++;
	stack_destroy(stack_); 
	return SUCCESS;
}

void shrink_buckets(hash_bucket *bucket,stack *stack_){
	int i;
	int pos;
	int *total=&(bucket->children_number);
	for(i=stack_->top-1;i>=0;i--){
		pos=get_stack_elements(stack_,i);
		//printf("shrink_buckets pos %d\n",pos);
		
		memmove(&(bucket->children[pos]),&(bucket->children[pos+1]),(*total-(pos+1))*sizeof(trie_node));
		//printf("INSIDE\n");
		*total=*total-1;
		//printf("Now children left on the bucket are %d\n",bucket->children_number);
		}
	/*if(*total<bucket->capacity-hash->bucket_capacity){ //freeing the realloced extra tries
			bucket->children=realloc(bucket->children,hash*sizeof(trie_node));
			}*/
}

int deleteTrieNode(hash_layer *hash,char **words,int word_number){
	int error;

	int hash_val=hash_function(hash,words[0]);
	//printf("hash val is %d\n",hash_val);

	trie_node *node;
	int pos;
	node=delete_from_backet(hash,hash_val,words[0],&pos); //node gets the value of the first trie node
	if(node==NULL) return ERROR;
	if(word_number>1){ 
		//printf("deleting more words\n");
		error=delete_ngram(node,words,1,word_number-1);
		if(error!=SUCCESS){
			return error;
		}
	
		if(node->number_of_childs!=0 || node->is_final=='y') return 2;
		hash_bucket *bucket=&(hash->buckets[hash_val]);
		int total=bucket->children_number;
		free(node->word);
		free(node->children);
		memmove(&(bucket->children[pos]),&(bucket->children[pos+1]),(total-(pos+1))*sizeof(trie_node));
		bucket->children_number--;
		return SUCCESS;
	}
	if(node->is_final=='n') return ERROR;
	if(node->number_of_childs!=0){ 
		node->is_final='n';
		return 2;
	}

	hash_bucket *bucket=&(hash->buckets[hash_val]);
	int total=bucket->children_number;
	free(node->word);
	free(node->children);
	memmove(&(bucket->children[pos]),&(bucket->children[pos+1]),(total-(pos+1))*sizeof(trie_node));
	bucket->children_number--;
	hash->total_children--;
	
	
	return SUCCESS;
}

trie_node *delete_from_backet(hash_layer *hash,int hash_val,char *word,int *pos){
	hash_bucket *bucket=&(hash->buckets[hash_val]);

	trie_node *node;
	//int exists=check_exists_in_bucket(bucket,word,pos);
	int exists=check_exists_in_bucket(word,pos,bucket->children,bucket->children_number);
	//printf("exists in delete is %d and word is %s and hash_val is %d\n",exists,word,hash_val);
	if(exists==0) return NULL;
	node=&(bucket->children[*pos]);
	return node;
}

void print_hash(hash_layer *hash){
	int i,j;
	hash_bucket bucket;
	trie_node node;
	for(i=0;i<hash->buckets_number;i++){
		bucket=hash->buckets[i];
		printf("Bucket[%d]::",i);
		for(j=0;j<bucket.children_number;j++){
			node=bucket.children[j];
			printf("-%s(%c)->",node.word,node.is_final);
			print_trie(&node,0);
		}
		printf("\n");
	}
}


int lookupTrieNode(hash_layer *hash,char **words,int number_of_words){
	//printf("Inside search,number of words is %d\n",number_of_words);
	stack *stack_=init_stack();
	int word_number;
	int exists;
	int pos;
	trie_node *node;
	int start=0;
	paths *paths_=init_paths(4,10); //rows columns
	while(start!=number_of_words+1) {
		//printf("\nstart with %s\n",words[start]);
		word_number=start;
		
		int hash_val=hash_function(hash,words[start]);

		hash_bucket *bucket=&(hash->buckets[hash_val]);
		//exists=check_exists_in_bucket(bucket,words[start],&pos);
		exists=check_exists_in_bucket(words[start],&pos,bucket->children,bucket->children_number);
		//printf("exists is %d\n",exists);
		if(exists==0){ 
			start++;
			continue;
			}

		node=&(bucket->children[pos]);
		push(stack_,hash_val);
		push(stack_,pos);
		while(node->number_of_childs!=0) {
			//printf("here\n");
			//printf("word number :%d %s\n",word_number,words[word_number]);
			if(node->is_final=='y') {
				//print_stack(stack_);
				check_in_paths4(paths_,stack_,hash); //I found it
				}
			//printf("word_number is %d\n",word_number);
			word_number++;
			if(word_number>number_of_words) break;
			exists=check_exists_in_children(node,words[word_number],&pos);
			if(exists==0) break;
			//printf("I am gonna push : %d\n",pos);
			push(stack_,pos);
			node=&(node->children[pos]);
			//word_number++;
		}
		if(exists==1 && word_number<=number_of_words) {
			//print_stack(stack_);
			check_in_paths4(paths_,stack_,hash);
			//print_nodes_from_hash(hash,stack_);
		}
		reset_stack(stack_);
		start++;
	}
	int found=SUCCESS;
	if(paths_->words_in==0) found=-1;
	stack_destroy(stack_);
	delete_paths(paths_); //rows	
	return found;
	if(exists==0) return ERROR;
	
	return SUCCESS;	


	return 0;
}



int lookupTrieNode_with_bloom(hash_layer *hash,char **words,int number_of_words,topk * top){
	//size_t bloomfilterbytes = ((M*128)/8);
	size_t bloomfilterbytes=M*8;
	//	int multi=number_of_words/M;
	//if(multi!=0) bloomfilterbytes = (M *(2<<(multi-1)));
	//printf("multi is %d with bytes %d with words %d\n",multi,bloomfilterbytes,number_of_words);
	int * bloomfilter = malloc(bloomfilterbytes/8);
	bloomfilter_init(bloomfilter,bloomfilterbytes);

	char *str;	
	int str_size;
	int word_number;
	int exists;
	int pos;
	trie_node *node;
	int start=0;
	int ngrams_found=0;
	while(start!=number_of_words+1) {

		str=malloc(20*sizeof(char)); //check this size
		strcpy(str,"");
		str_size=20;
		word_number=start;
		
		int hash_val=hash_function(hash,words[start]);

		hash_bucket *bucket=&(hash->buckets[hash_val]);
		//exists=check_exists_in_bucket(bucket,words[start],&pos);
		exists=check_exists_in_bucket(words[start],&pos,bucket->children,bucket->children_number);
		//printf("exists is %d\n",exists);
		if(exists==0){ 
			start++;
			free(str);
			continue;
			}

		node=&(bucket->children[pos]);
		//printf("words start %d , %s\n",start,words[start]);
		myappend_pan(&str,&str_size,words[start]);
		
		while(node->number_of_childs!=0) {
			
			if(node->is_final=='y') { //found ngram
				if(bloomfilter_check(str,bloomfilter,bloomfilterbytes)==0){
						bloomfilter_add(str,bloomfilter,bloomfilterbytes);
						top=add_top(top,str);
						ngrams_found++;
					}
				}
			//printf("word_number is %d\n",word_number);
			word_number++;
			if(word_number>number_of_words) break;
			exists=check_exists_in_children(node,words[word_number],&pos);
			if(exists==0) break;

			myappend_pan_with_space(&str,&str_size,words[word_number]);
					//printf("word after append is \"%s\" with len %d\n",str,strlen(str));
			node=&(node->children[pos]);
			//word_number++;
		}
		if(exists==1 && word_number<=number_of_words) {
			if(bloomfilter_check(str,bloomfilter,bloomfilterbytes)==0){
						bloomfilter_add(str,bloomfilter,bloomfilterbytes);
						top=add_top(top,str);
						ngrams_found++;
			}
		}
		free(str);
		start++;
	}
	end_gram_table(top,ngrams_found);
	int found=SUCCESS;
	//free(str);
	free(bloomfilter);	
	return found;
	if(TestAllBits(bloomfilter,bloomfilterbytes)==0) found=-1;
	if(exists==0) return ERROR;
	
	return SUCCESS;	


	return 0;
}


char * detableize(char * str, char ** table){
	int i;
	for (i=0;i<table_size;i++)
		str = myappend(str,table[i]);

return str;
}



void test(void){

printf("Hello from Test\n");

}
