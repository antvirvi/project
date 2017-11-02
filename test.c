#include "test.h"

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
