#include "functions.h"
#include "libraries.h"

extern int buffer_size;
extern int word_size;
extern int table_size;
int reset = 0;


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
		//append_trie_node_iterative(trie->root,ptr_table,0,words_in-1);
		append_trie_node(trie->root,ptr_table,0,words_in-1);
	}
	//printf ("free\n");
	free(line);
	cleanup(ptr_table);
	fclose(fd);
	return 0;	
}






int test_input(struct index *trie,char * filename)
{	
	//printf("\x1b[32m""TEST_INPUT start\n""\x1b[0m");
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

	//paths *paths_=init_paths(4,10); 

	for(a=0;a<table_size;a++)
		ptr_table[a]=malloc(word_size*sizeof(char));

	while ((read = getline(&line, &len, fd)) != -1) {
		//words_in = 1;
		words_in = 0;
		
		word = strtok (line," \n");
		while(word!=NULL){
			//printf("Read this word: %s\n",word);
			if(strcmp(word,"Q")==0){
				flag=1;
				//printf("\x1b[36m""Question 1\n""\x1b[0m");
			}
			else if(strcmp(word,"A")==0){
				//printf("\x1b[36m""Addition 2\n""\x1b[0m");
				flag=2;
			}
			else if(strcmp(word,"D")==0){
				//printf("\x1b[36m""Deletion 3\n""\x1b[0m");
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
				//printf("\x1b[36m""Empty word found as a countable word -1\n""\x1b[0m");
				
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

				//ptr_table[words_in] = malloc(word_size*sizeof(char));
			strcpy(ptr_table[words_in],word);
				
				words_in++;				
			}
			word=strtok(NULL," \n");
		}

		switch(flag){
			case 1 :
				printf("\n"); 
				command_error=search_in_trie(trie->root,ptr_table,words_in-1);
				if(command_error==-1) printf("%d\n",command_error);
				break;
			case 2 :
				//command_error=append_trie_node_iterative(trie->root,ptr_table,0,words_in-1);
				command_error=append_trie_node(trie->root,ptr_table,0,words_in-1);
				break;
			case 3 :
				command_error=delete_ngram(trie->root,ptr_table,0,words_in-1);
				//printf("error is %d \n",command_error);
				break;
		
		}
		flag=0;	
}
	//it is supposed that control never reaches this point, due to F signal
  	free(line);
	cleanup(ptr_table);
	fclose(fd);
	//printf("\x1b[32m""TEST_INPUT unpredicted end at end of function\n""\x1b[0m");
	
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
	//printf("Inside init trie\n");
	//print_node(node);
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
		//printf("inside check exists\n");
		//if(upper==-1) return 0; //i made this change
		while(1!=0){
			//printf("upper %d lower %d pivot %d\n",upper,lower,pivot);
			if(upper<=lower){
				pivot=(upper+lower)/2;
				int compare=strcmp(node->children[pivot].word,word);
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
				int compare=strcmp(node->children[pivot].word,word); // equal=0 children[i]<word: compare<0 children>word : compare>0
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

/**int delete_ngram(trie_node *node,char **word,int word_number,int number_of_words){
		int error;
		stack *stack_=init_stack();
		trie_node *root=node;
		printf("in delete ngram\n");
		while(word_number!=number_of_words){
			printf("word is %s\n",word[word_number]);
			if(node->number_of_childs==0) return ERROR;
			else{
				int pos;
				int exists=check_exists_in_children(node,word[word_number],&pos);
				if (exists==1){
					push(stack_,pos); //push the position on the stack to remove it later
					node=&(node->children[pos]);
					word_number=word_number+1;
				}
				else error=1;	//error is 1 if the word is not on the trie , so the ngram is not in the trie
				}
		}
		print_stack_(stack_);
		if(node->number_of_childs!=0) return ERROR; //check if there are childs ont the node and return error if there are
		 
		return SUCCESS;//and return no error . the previous one is gonna delete it
}*/

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


int search_in_trie(trie_node *root,char **word,int number_of_words){
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

/*int check_in_paths(paths *paths_, stack *stack_,trie_node *root){//initialize paths in -1
	//printf("inside check in paths\n");
	int number=get_stack_number(stack_);
	int path_pos=0;
	int found=1;
	int pos,i,j;
	for(i=0;i<number;i++){
		pos=get_stack_elements(stack_,i);
		for(j=0;j<paths_->words_in;j++){
			if(paths_->paths_array[j][path_pos]==pos){
				path_pos++;
				break;
			} //if it finds it continue with the next word
		}
		if(j==paths_->words_in) {found=0; break;} //if it doesnt find one node f the path then leave
 			
	}
	if(found==0) {
		add_to_paths(paths_,stack_); //add to paths and print it
		print_nodes_from_stack(root,stack_);
		
	}
	return found;
}*/

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
		print_nodes_from_stack(root,stack_);
		
	}
	return found;
}

/*int check_in_paths2(paths *paths_, stack *stack_,trie_node *root){//initialize paths in -1
	//printf("inside check in paths\n");
	int number=get_stack_number(stack_); //how many words the phrase has
	int path_pos=0; //in which word on the paths i am in 
	int found=0; //if the phrase path is found in paths
	int pos; //pos in trie of the word of the phrase    
	int i=0;
	int j=0;
	int first_pos=get_stack_elements(stack_,i);
	while(paths_->paths_array[j][path_pos]<=first_pos && j!=paths_->words_in){
		for(i=1;i<number;i++){
			pos=get_stack_elements(stack_,i);
			if(paths_->paths_array[j][i]!=pos) break; //if it finds it continue with the next word
			}
		if(i==number) {found=1; break;}
		j++;
		}
	
 	if(found==0) {
		add_to_paths2(paths_,stack_); //add to paths and print it
		print_nodes_from_stack(root,stack_);
	}		
	return found;
}


void add_to_paths2(paths *paths_, stack *stack_){
	int path_num=paths_->words_in;
	if(path_num==paths_->max_words) double_paths(paths_);
	int number=get_stack_number(stack_);
	int i ,pos;
	i=0;
	//printf("Found N gram: ");
	pos=get_stack_elements(stack_,i);
	if(i==paths_->words_in)i=0;
	else{
		while(paths_->paths_array[i][0]<pos && i<paths_->words_in) i++; //find first position
		}
	

	path_num=i;
	printf("\npath num is %d\n",path_num);
	printf("words_in are %d\n",paths_->words_in);
	if(path_num!=paths_->words_in && paths_->words_in!=0){
		//printf("in move : %d\n",(paths_->words_in-path_num));
		int **backup=paths_->paths_array;
		//memmove(paths_->paths_array,backup,path_num*sizeof(backup[0]));
		memmove(paths_->paths_array+(path_num+1),backup+path_num,(paths_->words_in-(path_num))*sizeof(backup[0]));

	}
	//paths_->words_in++;
	//print_paths(paths_);
	for(i=0;i<number;i++){
		pos=get_stack_elements(stack_,i);
		paths_->paths_array[path_num][i]=pos;
	}
	//printf("|");
	paths_->words_in++;
	print_paths(paths_);
}

*/

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

void initialize_bucket(hash_bucket *bucket,int m){
	bucket->children=malloc(m*sizeof(trie_node));
	bucket->overflow_bucket=NULL;
	bucket->children_number=0;
}

hash_layer	*createLinearHash(int c ,int m){ //c is number of buckets ,m is number of cells per bucket
	int i;
	hash_layer *hash=malloc(sizeof(hash_layer));
	if(hash==NULL) return NULL;

	hash->bucket_capacity=m;
	hash->buckets_number=c;
	hash->load_factor=0.5;
	hash->total_children=0;
	hash->bucket_to_split=0;
	hash->split_round=1;
	hash->buckets=malloc(c*sizeof(hash_bucket));
	if(hash->buckets==NULL) return NULL;
	
	for(i=0;i<c;i++){
		initialize_bucket(&(hash->buckets[i]),m);
	}
	return hash;
}

int  hash_function(int hash_buckets, char *word)
{
    unsigned long hash = 5381;
    int c;

    while (c = *word++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash%hash_buckets;
}


/*int hash_function(int hash_buckets,char *word){  //check 
	unsigned long int hashval;
	int i = 0;


	while( hashval < 100 && i < strlen(word) ) {
		hashval = hashval << 8;
		hashval += word[i];
		i++;
	}

	return hashval % hash_buckets;

}*/

int destroyLinearHash(){
	return 0;
}

int insertTrieNode(hash_layer *hash,char **words,int word_number){
	char is_final='n';
	if(word_number==1) is_final='y';
	int hash_val=hash_function(C*hash->split_round,words[0]);
	trie_node *node;
	node=add_to_backet(hash,hash_val,words[0],is_final); //node gets the value of the first trie node
	//print_node(node);
	printf("node word is %s\n",node->word);
	if(word_number>1){ 
		printf("appending more words\n");
		append_trie_node(node,words,1,word_number-1);}	
	
	return 0;
}



trie_node* add_to_backet(hash_layer *hash,int hash_val,char *word,char is_final){
	//hash_layer *hash=root->hash;
	hash_bucket *bucket=&(hash->buckets[hash_val]);
	
	int *last=&(bucket->children_number);
	trie_node *node;

	//check if the word already exists

	//this happens only once make it happen as it needs
	while(bucket->overflow_bucket!=NULL) bucket=bucket->overflow_bucket; //going to the last bucket

	if(*last==hash->bucket_capacity && bucket->overflow_bucket==NULL){ //initializing overflow bucket
		bucket->overflow_bucket=malloc(sizeof(hash_bucket));
		if(bucket->overflow_bucket==NULL) return NULL;
		initialize_bucket(bucket->overflow_bucket,hash->bucket_capacity);
		printf("I made a new overflow bucket at bucket %d\n",hash_val);
	} 
	if(*last==hash->bucket_capacity && bucket->overflow_bucket!=NULL) { //*las>capacity
		bucket=bucket->overflow_bucket;
		last=&(bucket->children_number);
	}

	node=&(bucket->children[*last]);
	init_trie_node(node,word,is_final);
	*last=*last+1;
	hash->total_children++;
	//check for splitting
	/*if((hash->total_children/(float)hash->buckets_number) > hash->load_factor){
		printf("attempting to expand buckets\n");
		hash->buckets=realloc(hash->buckets,(hash->buckets_number+1)*sizeof(hash_bucket)); //add bucket lineat
		if(hash->buckets==NULL){
			printf("error in realloc\n");
			return NULL;
		}
		hash->buckets_number++;
		//initialize bucket
		initialize_bucket(&(hash->buckets[hash->buckets_number-1]),hash->bucket_capacity);
		int i;
		hash_bucket *new_bucket=&(hash->buckets[hash->buckets_number-1]); //pointer to the new bucket
		bucket=&(hash->buckets[hash->bucket_to_split]); //re arranging bucket to split
		int new_hash_val=-1;
		stack *stack_=init_stack();
		print_stack(stack_);

		if(bucket->children_number==0){
			hash->bucket_to_split=(hash->bucket_to_split+1)%(hash->split_round*C);// 
			return node;
			} // no need for rearranging bucket

		//re arange buucket
		while(bucket!=NULL || new_hash_val==-1){ //bucket->overflow_bucket!=NULL
			printf("attempting to re arrange bucket %d and %d\n",hash->bucket_to_split,hash->buckets_number-1);
			push(stack_,-1);
			print_stack(stack_);
			for(i=0;i<bucket->children_number;i++){

				new_hash_val=hash_function(C*(hash->split_round+1),bucket->children[i].word);
				if(new_hash_val==hash_val) continue; //if hash value is the same then no need to change bucket

				if(new_bucket->children_number==hash->bucket_capacity){	//if new bucket fills then create an overflow bucket
					new_bucket->overflow_bucket=malloc(sizeof(hash_bucket));
					if(new_bucket->overflow_bucket==NULL) return NULL;	
					initialize_bucket(new_bucket->overflow_bucket,hash->bucket_capacity);
					new_bucket=new_bucket->overflow_bucket;
				}

				memmove(&(new_bucket->children[new_bucket->children_number]),&(bucket->children[i]),sizeof(trie_node)); //copy nod
				push(stack_,i);		
				print_stack(stack_);
				
				new_bucket->children_number++;
				
			}
			bucket=bucket->overflow_bucket;
		}

		print_stack(stack_);
		shrink_buckets(&(hash->buckets[hash->bucket_to_split]),stack_);
		hash->bucket_to_split=(hash->bucket_to_split+1)%(hash->split_round*C);//
		printf("hash after arrange\n");
		print_hash(hash);
	}*/
	resize_hash(hash,hash_val);
	return node;
	//return SUCCESS;
}

int resize_hash(hash_layer *hash,int hash_val){
	hash_bucket *bucket;
	if((hash->total_children/(float)hash->buckets_number) > hash->load_factor){
		printf("attempting to expand buckets\n");
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
		print_stack(stack_);

		if(bucket->children_number==0){
			hash->bucket_to_split=(hash->bucket_to_split+1)%(hash->split_round*C);// 
			return SUCCESS;
			} // no need for rearranging bucket

		//re arange buucket
		while(bucket!=NULL || new_hash_val==-1){ //bucket->overflow_bucket!=NULL
			printf("attempting to re arrange bucket %d and %d\n",hash->bucket_to_split,hash->buckets_number-1);
			push(stack_,-1);
			print_stack(stack_);
			for(i=0;i<bucket->children_number;i++){

				new_hash_val=hash_function(C*(hash->split_round+1),bucket->children[i].word);
				if(new_hash_val==hash_val) continue; //if hash value is the same then no need to change bucket

				if(new_bucket->children_number==hash->bucket_capacity){	//if new bucket fills then create an overflow bucket
					new_bucket->overflow_bucket=malloc(sizeof(hash_bucket));
					if(new_bucket->overflow_bucket==NULL) return ERROR;	
					initialize_bucket(new_bucket->overflow_bucket,hash->bucket_capacity);
					new_bucket=new_bucket->overflow_bucket;
				}

				memmove(&(new_bucket->children[new_bucket->children_number]),&(bucket->children[i]),sizeof(trie_node)); //copy nod
				push(stack_,i);		
				print_stack(stack_);
				
				new_bucket->children_number++;
				
			}
			bucket=bucket->overflow_bucket;
		}

		print_stack(stack_);
		shrink_buckets(&(hash->buckets[hash->bucket_to_split]),stack_);
		hash->bucket_to_split=(hash->bucket_to_split+1)%(hash->split_round*C);//
		printf("hash after arrange\n");
		print_hash(hash);
	}
	return SUCCESS;
}

void shrink_buckets(hash_bucket *bucket,stack *stack_){
	int i;
	int pos;
	int first=0;
	//hash_bucket *temp=bucket;
	for(i=1;i<stack_->top;i++){ //first is always -1
		pos=get_stack_elements(stack_,i);
		printf("shrink_buckets pos %d\n",pos);
		if(pos==-1 && first-i!=1){
			shrink_bucket(bucket,stack_,first,i);
			bucket->children_number=bucket->children_number-(i-first-1); //new children number
			printf("Now children left on the bucket are %d\n",bucket->children_number);
			first=i;
			bucket=bucket->overflow_bucket;
		}
	}
}

void shrink_bucket(hash_bucket *bucket,stack *stack_,int first,int last){
	//delete nod from the old bucket
				//if the next is not the last then memove whats left
				//otherwise dont do it 
	int i,pos;
	int total=bucket->children_number;
	printf("total number here %d\n",total);
	for(i=last-1;i>first;i--){ //this may cause problem
		pos=get_stack_elements(stack_,i);
		memmove(&(bucket->children[pos]),&(bucket->children[pos+1]),(total-(pos+1))*sizeof(trie_node));
	}
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
			printf("%s(%c)->",node.word,node.is_final);
			print_trie(&node,0);
		}
		while(bucket.overflow_bucket!=NULL){ //prit overflow buckets
			printf("-overflow-");
			bucket=*(bucket.overflow_bucket);
			for(j=0;j<bucket.children_number;j++){
				node=bucket.children[j];
				printf("%s(%c)->",node.word,node.is_final);
				print_trie(&node,0);
			} 
		}
		printf("\n");
	}
}


int lookupTrieNode(){
	return 0;
}
