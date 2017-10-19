#include "functions.h"

int main (void)
{
	int error;
	char **word=malloc(4*sizeof(char*));
	word[0]=malloc(WORD_SIZE*sizeof(char));
	word[1]=malloc(WORD_SIZE*sizeof(char));
	word[2]=malloc(WORD_SIZE*sizeof(char));
	word[3]=malloc(WORD_SIZE*sizeof(char));
	strcpy(word[0],"the");
	strcpy(word[1],"Parrot");
	strcpy(word[2],"is");
	strcpy(word[3],"red");
	


	trie_node *root=init_trie();
	print_node(root);
	printf("max childss are %d\n",root->max_childs);
 	append_trie_node(root,word,0,0);
	
	strcpy(word[0],"panos");
	strcpy(word[1],"Parrot");
	strcpy(word[2],"is");
	strcpy(word[3],"blue");
	
	error=append_trie_node(root,word,0,0);
	printf("error is %d\n",error);
	printf("max childs are %d\n",root->max_childs);

	printf("\n");
	//print_node(root->children[0]);
	//print_node(root->children[1]);
	print_trie(root,0);
	delete_ngram(root,word,0,0);	
	print_trie(root,0);
	
}
