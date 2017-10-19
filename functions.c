#include "functions.h"

void printpanos(void)
{
	printf("Hello Panos\n");
	
}

void print_node(trie_node *node){
	printf("node word is %s\n",node->word);
	printf("node has %d children\n",node->number_of_childs);
	printf("node is final: %c\n",node->is_final);
}

void print_trie(trie_node *node,int level){
	int i;
	if(node->number_of_childs==0) return;
	printf("In level %d : \n",level );
	for(i=0;i<node->number_of_childs;i++){
		print_node(node->children[i]);
		print_trie(node->children[i],level+1);
	}
	return;
}

trie_node *init_trie(){
	int i;
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
	int i;
	trie_node *node=malloc(sizeof(trie_node));
	node->word=malloc(WORD_SIZE*sizeof(char));
	strcpy(node->word,word);
	
	node->is_final=is_final;
	node->number_of_childs=0;
	node->max_childs=MAX_CHILDS;


	node->children=malloc(CHILD_NUM*sizeof(struct trie_node));
	for(i=0;i<CHILD_NUM;i++) node->children[i]=NULL;

	return node;
}

trie_node *init_trie_node(trie_node *node,char *word,char is_final){
	int i;
	//trie_node *node=malloc(sizeof(trie_node));
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
		append_trie_node(root->children[0],word,word_number+1,number_of_words);
	}
	else{
		int pos;
		int exists=check_exists_in_children(root,word[word_number],&pos);
		if (exists==1){
			//make the node also final
			(root->children[pos])->is_final=is_final; 
			append_trie_node(root->children[pos],word,word_number+1,number_of_words);
			}
		else{	
			error=append_word(root,pos,word[word_number],is_final);
			if(error==ERROR) return ERROR;
			root->number_of_childs++;
			append_trie_node(root->children[pos],word,word_number+1,number_of_words);
			}
	}
	return SUCCESS; //0 errors
}

int check_exists_in_children(trie_node *node,char *word,int *pos){
		int pivot; //pivor is integer so in the division it will rounf in the smaller absolute value 5/2=2
		int lower=0;
		int upper=node->number_of_childs;
		//printf("inside check exists\n");
		while(1!=0){
			//printf("upper %d lower %d pivot %d\n",upper,lower,pivot);
			if(upper==lower || upper<lower || lower>upper){
				*pos=pivot;
				return 0; //not exact match
				}
			else {
				pivot=(upper+lower)/2;
				int compare=strcmp((node->children[pivot])->word,word); // equal=0 children[i]<word: compare<0 children>word : compare>0
				//printf("compare is %d\n",compare); 
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
				node->children=realloc(node->children,node->max_childs*2);
				if(node->children==NULL) return ERROR;
		        node->max_childs*=2;	
		}
		
		trie_node ** backup=node->children;
		memmove(node->children,backup,pos*sizeof(trie_node*));
		memmove(node->children+pos+1,backup+pos,(node->number_of_childs-pos)*sizeof(trie_node*));
		
		trie_node *child=create_trie_node(word,is_final);
		if(child==NULL) return ERROR;
		node->children[pos]=child;
		//node->children[pos]=init_trie_node(node->children[pos],word,is_final);
		return SUCCESS ; //zero errors
}

int delete_ngram(trie_node *root,char **word,int word_number,int number_of_words){
		int error;
		printf("in delete ngram\n");
		if(word_number==number_of_words){
			if(root->number_of_childs!=0) return ERROR; //check if there are childs ont the node and return error if there are 
			return SUCCESS;//and return no error . the previous one is gonna delete it	
		}
		if(root->number_of_childs==0) return ERROR;
		else{
			int pos;
			int exists=check_exists_in_children(root,word[word_number],&pos);
			if (exists==1){
				error=delete_ngram(root->children[pos],word,word_number+1,number_of_words);
				if(error==0) //delete
				delete_from_node(root,pos);
				return error; 
			}
			else return ERROR;	//return error if the word is not on the trie , so the ngram is not in the trie
			}
}

int delete_from_node(trie_node *node,int pos){
		trie_node ** backup=node->children;
		memmove(node->children,backup,(pos-1)*sizeof(trie_node*));
		memmove(node->children+pos,backup+pos,(node->number_of_childs-(pos))*sizeof(trie_node*));
		
		return SUCCESS ; //zero errors
}



