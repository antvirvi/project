#include "functions.h"
#include "libraries.h"

extern int buffer_size;
extern int word_size;
extern int table_size;
int reset = 0;



int init_input(struct index *trie,char * filename){
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
		word = strtok (line," ");
		while(word!=NULL)
		{
		//printf("Read this word: %s\n",word);
			if(words_in==table_size - 1){
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
			word=strtok(NULL," ");
		}
		printf("ptr_table %s %d\n",ptr_table[0],words_in);
		append_trie_node(trie->root,ptr_table,0,words_in-1);	
	}
	free(line);
	//cleanup(ptr_table);
	return 0;	
}



int test_input(struct index *trie,char * filename)
{	
	char **ptr_table = malloc(table_size*sizeof(char *));
	int words_in = 0;
	int flag; //1 question, 2 addition, 3 deletion, 4 end of file
	//char* buffer =  malloc(buffer_size*sizeof(char));  //creating a char table to store orders from input file
	int a;
	FILE* fd = fopen(filename, "r"); //opening input file
	//strcpy(buffer,"\0");

	if(fd == NULL)
	{
		perror("Error opening input file");
		return -1;
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *word;


	for(a=0;a<table_size;a++)
		ptr_table[a]=malloc(word_size*sizeof(char));


	while ((read = getline(&line, &len, fd)) != -1) {	words_in = 0;
	       // printf("Retrieved line of length %zu :\n", read);
	       // printf("%s", line);
		word = strtok (line," ");
		while(word!=NULL){
			
			//printf("Read this word: %s\n",word);
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
				cleanup(ptr_table);
				exit(1);
			}
			else{
				
				if(words_in==table_size - 1){
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
				//ptr_table[words_in] = malloc(word_size*sizeof(char));
				strcpy(ptr_table[words_in],word);
				words_in++;
			}
			word=strtok(NULL," ");		
			
		}
		switch(flag){
			case 1 :
				//add ptr_table to trie PANOS
				//append_trie_node(root,word,0,0);
				break;
			case 2 :
				//	error=delete_ngram(root,word,0,3);//third argument is always 0 , fourth is lenof(words) -1
				//delete ptr_table from trie PANOS
				break;
			case 3 :
				//search trie for this ptr_table PANOS
				break; 
		
		}
		flag=0;
	}
  	free(line);
    
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

trie_node *create_trie_node(char *word,char is_final){
	trie_node *node=malloc(sizeof(trie_node));
	node->word=malloc(WORD_SIZE*sizeof(char));
	strcpy(node->word,word);
	
	node->is_final=is_final;
	node->number_of_childs=0;
	node->max_childs=MAX_CHILDS;


	node->children=malloc(CHILD_NUM*sizeof(struct trie_node));

	return node;
}

trie_node *init_trie_node(trie_node *node,char *word,char is_final){
	printf("Inside init trie\n");
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
		printf("out of words to add\n");
		return SUCCESS;
		}
	printf("append trie node %d out of %d word: %s\n",word_number,number_of_words,word[word_number]);
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
			(&(root->children[pos]))->is_final=is_final; 
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

int check_exists_in_children(trie_node *node,char *word,int *pos){
		int pivot; //pivor is integer so in the division it will rounf in the smaller absolute value 5/2=2
		int lower=0;
		int upper=node->number_of_childs-1;
		//printf("inside check exists\n");
		while(1!=0){
			//printf("upper %d lower %d pivot %d\n",upper,lower,pivot);
			if(upper<=lower){
				pivot=(upper+lower)/2;
				int compare=strcmp(node->children[pivot].word,word);
				if(compare==0){
					*pos=pivot;
					return 1; //exact match
				}	
				*pos=(compare<0)? lower+1:lower;
				return 0; //not exact match
				}
			else {
				pivot=(upper+lower)/2;
				//printf("pivot here is %d\n",pivot);
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
		printf("inside append_word , pos %d\n",pos);
		if(node->number_of_childs==node->max_childs){
				printf("I have to double the children\n");
				node->children=realloc(node->children,node->max_childs*2*sizeof(trie_node));
				printf("Done Realloc \n");
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
		//printf("in delete ngram word %s\n",word[word_number]);
		if(word_number==number_of_words+1){
			if(root->number_of_childs!=0) return ERROR; //check if there are childs ont the node and return error if there are 
			return SUCCESS;//and return no error . the previous one is gonna delete it	
		}
		if(root->number_of_childs==0) return ERROR;
		else{
			int pos;
			int exists=check_exists_in_children(root,word[word_number],&pos);
			if (exists==1){
				error=delete_ngram(&(root->children[pos]),word,word_number+1,number_of_words);
				if(error==0)
				{
					if(root->children[pos].is_final=='y' && word_number!=number_of_words) return 2; //return 2 if  i am del half 
					error=delete_from_node(root,pos);
					root->number_of_childs--;
				}
				return error; 
			}
			else return ERROR;	//return error if the word is not on the trie , so the ngram is not in the trie
			}
}



int delete_from_node(trie_node *node,int pos){
		//printf("in delete node pos %d word to delete is %s\n",pos,node->children[pos].word);

		trie_node * backup=node->children;
		memmove(node->children,backup,pos*sizeof(trie_node));
		memmove(node->children+pos,backup+pos+1,(node->number_of_childs-(pos+1))*sizeof(trie_node));
		//printf("out of delete\n");
		return SUCCESS ; //zero e-rrors
}


int search_in_trie(trie_node *root,char **word,int number_of_words){
	
	stack *stack_=init_stack();
	int word_number=0;
	int exists;
	int pos;
	trie_node *node=root;
	while(node->children!=NULL && word_number!=number_of_words){
		exists=check_exists_in_children(node,word[word_number],&pos);
		if(exists==0) break;
		push(stack_,pos);
		node=&(node->children[pos]);
	}
	if(exists==0){
		stack_destroy(stack_);
		return ERROR;
	}
	print_stack(stack_);
	stack_destroy(stack_);
	return SUCCESS;	


}
 




