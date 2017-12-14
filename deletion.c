
#ifndef DEL_H
#define DEL_H
#include "deletion.h"
#endif

void init_ngrams_to_delete(ngrams_to_delete *d_grams){
	d_grams->nodes_capacity=10; // capacity of 1 d array
	d_grams->length_capacity=10;	
	d_grams->pos=0;
	d_grams->length=malloc(d_grams->length_capacity*sizeof(int)); // in each deletion 
	memset(d_grams->length,0,d_grams->length_capacity*sizeof(int));
	d_grams->nodes_to_delete=malloc(d_grams->nodes_capacity*sizeof(trie_node*));
	d_grams->children_to_delete=malloc(d_grams->nodes_capacity*sizeof(trie_node*));
}

void add_ngram_to_delete(ngrams_to_delete *d_grams,trie_node *node,int child_pos,int ngram){
	if(ngram==d_grams->length_capacity){
		d_grams->length=realloc(d_grams->length,d_grams->length_capacity*2*sizeof(int)); // in each deletion 
		memset(&(d_grams->length[d_grams->length_capacity]),0,d_grams->length_capacity*sizeof(int));	
		d_grams->length_capacity*=2;				
	}
	d_grams->length[ngram]++;
	if(d_grams->nodes_capacity==d_grams->pos){
			d_grams->nodes_capacity*=2;
			d_grams->nodes_to_delete=realloc(d_grams->nodes_to_delete,d_grams->nodes_capacity*sizeof(trie_node*));
			d_grams->children_to_delete=realloc(d_grams->children_to_delete,d_grams->nodes_capacity*sizeof(trie_node*));
	}
	d_grams->nodes_to_delete[d_grams->pos]=node;
	d_grams->children_to_delete[d_grams->pos]=child_pos;
	d_grams->pos++;
}

void reset_ngrams_to_delete(ngrams_to_delete *d_grams){
	d_grams->pos=0;
	memset(d_grams->length,0,d_grams->length_capacity*sizeof(int));
}
void delete_ngrams(ngrams_to_delete *d_grams){
	int i=0;
	int start=0;
	int j;
	trie_node *node;
	while(d_grams->length[i]!=0){
		for(j=start;j<start+d_grams->length[i];j++){
			node=d_grams->nodes_to_delete[j];
			delete_from_node(node,d_grams->children_to_delete[j],0);
			node->children_deleted--;
		}
		start=start+d_grams->length[i];
		i++;
	}

}


void destroy_ngrams_to_delete(ngrams_to_delete *d_grams){
	free(d_grams->length);
	free(d_grams->nodes_to_delete);
	free(d_grams->children_to_delete);
}
