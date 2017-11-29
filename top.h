#ifndef VAR
#define VAR
#include "libraries.h"
#endif

extern int table_ngram_size;

typedef struct kframes{
int capacity; 	//posa xwrane
int occupied; 	//posa exoume mesa
char ** ngrams; //deiktis se pinaka me ta ngrams
int  *  k; 		//pinakas me counter emfanisewn gia ta top k
int   q; 		//counter gia to posa q exoun perasaei. diladi poses allages grammis anamesa se 2 "F"
int * ends;		//pinakas me counter gia to meta apo poses lekseis allazei to guery
}kframes;


typedef struct freq{
	int *  frequency;
	char ** ngram;
}freq;


typedef struct index_table{
	freq * fr;
	char ** ngram;
}index_table;




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
