#include "top.h"

//extern int table_ngram_size;
/*
typedef struct kframes{
int capacity;
int occupied;
char ** ngrams;
int  *  k;
};
*/
//table holding all the ngramms
kframes *create_gram_table(kframes * kf){

	kf = malloc(sizeof(kframes));
	kf->ngrams = malloc(table_ngram_size*sizeof(char *));
	kf->k = malloc(table_ngram_size*sizeof(int));
	kf->capacity = table_ngram_size;
	kf->occupied = 0;

return kf;
}

kframes *extend_gram_table(kframes * kf){
	table_ngram_size*=2;
	kf->ngrams = realloc(kf->ngrams,table_ngram_size*sizeof(char *));
	kf->k = realloc(kf->k,table_ngram_size*sizeof(int));
	kf->capacity = table_ngram_size;

return kf;
}

kframes *init_gram_table(kframes * kf){

	kf->occupied = 0;

return kf;
}

kframes *add_gram_table(kframes * kf,char * ngram){
	if(kf->occupied==kf->capacity)
		kf = extend_gram_table(kf);

	kf->ngrams[kf->occupied] = malloc(sizeof(ngram));
	strcpy(kf->ngrams[kf->occupied],ngram);
	kf->occupied++;

return kf;
}
/*
kframes *newline_gram_table(kframes * kf){
	if(occupied==capacity)
		kf = extend_gram_table(kf);

	kf->ngrams[kf->occupied] = malloc(2);
	strcpy(kf->ngrams[kf->occupied],"\n");
	occupied++;

return kf;
}
*/
void print_gram_table(kframes *kf){

	int i;
	for(i=0;i<kf->capacity;i++){
		printf("%s",kf->ngrams[i]);
	}
}


void erase_gram_table(kframes * kf){
	int i;
	for(i=0;i<kf->capacity;i++){
		free(kf->ngrams[i]);
	}
	free(kf->k);
	free(kf);
}



//table holding the top appearances
void create_top_table(){


}



