#ifndef VAR
#define VAR
#include "libraries.h"
#endif

extern int table_ngram_size;

typedef struct kframes{
int capacity;
int occupied;
char ** ngrams;
int  *  k;
int   q;
int * ends;
}kframes;


kframes *create_gram_table(kframes * kf);
kframes *extend_gram_table(kframes * kf);
kframes *init_gram_table(kframes * kf);
kframes *add_gram_table(kframes * kf,char * ngram);
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
void print_gram_table(kframes *kf);
void end_gram_table(kframes *kf);

void erase_gram_table(kframes * kf);
