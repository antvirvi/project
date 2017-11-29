#include "functions.h"

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

/*extern*/ int buffer_size = 16;
/*extern*/ int word_size = 8;
/*extern*/ int table_size = 4;

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
		printf("File name %s\n",filename);
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
			words_in++;
			word=strtok(NULL," \n");
		}
		//append_trie_node_iterative(trie->root,ptr_table,0,words_in-1);
		append_trie_node(trie->root,ptr_table,0,words_in-1);
	}
	free(line);
	cleanup(ptr_table);
	fclose(fd);
	return 0;	
}


int test_input(struct index *trie,char * filename)
{/*
	int bloomfilterbytes = (M/8);
	printf("SHould keep %lu cells\n",(M/sizeof(int))/8);
//	int  bloomfilter[bloomfilterbits];
	int * bloomfilter = malloc(bloomfilterbytes);
	bloomfilter_init(bloomfilter); 
	printf("bloomfilter initialized\n");

	bloomfilter_add("antonis and banos is good",bloomfilter); //add a message to bloomfilter
// 	testcheck("2antonis and banos is good",bloomfilter); //check if a message is stored in bloomfilter
	if(bloomfilter_check("antonis and banos is good",bloomfilter)==0)
		printf("The test message is not stored\n");
	else
		printf("the string may be stored\n");
	bloomfilter_init(bloomfilter); //intialized bloomfilter to 0
	printf("middle\n");
	printf("%d\n",TestAllBits(bloomfilter)); //check if bloomgilter is 0 or not
	if(bloomfilter_check("antonis and banos is good",bloomfilter)==0)
		printf("The test message is not stored\n");
	else
		printf("the string may be stored\n");
*/
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

	kframes *kfrm=NULL;  //struct for the top k frames and printing after F
	kfrm = create_gram_table(kfrm);
	kfrm = init_gram_table(kfrm);

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *word;
	int command_error;
//	char * ngram;
	//paths *paths_=init_paths(4,10); 

	for(a=0;a<table_size;a++)
		ptr_table[a]=malloc(word_size*sizeof(char));

	while ((read = getline(&line, &len, fd)) != -1) {
		//words_in = 1;
		words_in = 0;
		//bloomfilter_init(bloomfilter);	//in every read of line we zero the bloom filter.
		
		//printf(YELLOW"Reset bloomfilter\n"RESET);
		//free(bloomfilter);
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
				print_gram_table(kfrm);
//				erase_gram_table(kfrm);
				init_gram_table(kfrm);
				
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
				while(strlen(word)>=word_size){
					word_size*=2;
					for(a=0;a<table_size;a++)
						ptr_table[a] = realloc(ptr_table[a],word_size*sizeof(char));
				}		

				//ptr_table[words_in] = malloc(word_size*sizeof(char));
			strcpy(ptr_table[words_in],word);
//			printf("|%s",word);
				
				words_in++;				
			}
			word=strtok(NULL," \n");
		}

		switch(flag){
			case 1 :
				//printf("\n"); 
				command_error=search_in_trie(trie->root,ptr_table,words_in-1,kfrm);   //AYTO EDW NA VGEI APO COMMENTS
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
	erase_gram_table(kfrm);
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


int search_in_trie(trie_node *root,char **word,int number_of_words,kframes * kf){
	//printf("Inside search\n");
	//stack *stack_=init_stack();

	size_t bloomfilterbytes = (M/8);
	int * bloomfilter = malloc(bloomfilterbytes);
	bloomfilter_init(bloomfilter);

	char *str;;
	int word_number;
	int exists;
	int pos;
	trie_node *node;
	int start=0;

	while(start!=number_of_words+1) {
		str=malloc(20*sizeof(char));
		strcpy(str,"");
		word_number=start;
		node=root;
		while(node->number_of_childs!=0) {
			//printf("word number :%d %s\n",word_number,word[word_number]);
			if(node->is_final=='y') {
					if(bloomfilter_check(str,bloomfilter)==0){
					//	printf("%s|",str); 
						bloomfilter_add(str,bloomfilter);
						kf = add_gram_table(kf,str);
									
					}
				}
			exists=check_exists_in_children(node,word[word_number],&pos);
			if(exists==0) 
				break;
			str = myappend(str,word[word_number]);
			node=&(node->children[pos]);
			word_number++;
		}
		if(exists==1) {
			if(bloomfilter_check(str,bloomfilter)==0){
					//	printf("%s|",str); 
						bloomfilter_add(str,bloomfilter);
						kf = add_gram_table(kf,str);
					}
		}
//		memset(0,str,sizeof(str));
//		str[0]='\0';
//		end_gram_table(kf);
		free(str);
		//reset_stack(stack_);
		start++;
	}
	//printf("\n");
	end_gram_table(kf);
	int found=SUCCESS;
	if(TestAllBits(bloomfilter)==0) found=-1;
	//print_paths(paths_);
//	free(str);
	free(bloomfilter);
	return found;
	if(exists==0) return ERROR;
//	free(str);
	
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
		print_nodes_from_stack(root,stack_);
		
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

char * myappend_1(char * string, char * word){
	if(string==NULL) 
		string = malloc(0);
	char *string2;
	string2 = malloc(strlen(string)+strlen(word)+2);
	strcpy(string2,string);
	strcat(string2," ");
	strcat(string2,word);
	size_t new_length = strlen(string)+strlen(word)+2;
	string =  realloc(string,new_length);
	strcpy(string,string2);
	free(string2);

	return string;
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

char * detableize(char * str, char ** table){
	int i;
	for (i=0;i<table_size;i++)
		str = myappend(str,table[i]);

return str;
}

void test(void){  //test function to call other functions instead of main
char * str ="antonis";
if(str[1]=='a')
	printf("hot chocolate\n");
}
