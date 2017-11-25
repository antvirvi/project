#include "top.h"

 int table_ngram_size = 30;
/*
typedef struct kframes{
int capacity;
int occupied;
char ** ngrams;
int  *  k;
int   q;
int * ends;
}kframes;

*/
//table holding all the ngramms
kframes *create_gram_table(kframes * kf){

	kf = malloc(sizeof(kframes));
	kf->ngrams = malloc(table_ngram_size*sizeof(char *));
	kf->k = malloc(table_ngram_size*sizeof(int));
	kf->capacity = table_ngram_size;
	kf->occupied = 0;
	kf->q = 0;
	kf->ends = malloc(sizeof(int*));

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
	kf->q = 0;

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
	int j=0;
	int * ptr;
	ptr = kf->ends;
	for(i=0;i<kf->occupied;i++){ 
		printf("%s",kf->ngrams[i]);  //ean ftasoume sto telos enos q prepei na orisoume tin allagi gramis kai to oxi "|"
		if(ptr[j]==i){
			printf("\n");
			j++;
		}
		else
			printf("|");
	}
}

void end_gram_table(kframes *kf){
	kf->ends[kf->q]=(kf->occupied)-1;
	kf->ends = realloc(kf->ends,((kf->q)+1)*(sizeof(int)));
	kf->ends[(kf->q)+1] = -1;
	kf->q++;
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



