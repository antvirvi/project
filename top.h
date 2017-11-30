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
	int ngram;
}freq;


typedef struct index_table{
	int fr_index;
	int ngram;
	struct index_table * next;
}index_table;


kframes *create_gram_table(kframes * kf);
kframes *extend_gram_table(kframes * kf,freq * fre,index_table** it);
kframes *init_gram_table(kframes * kf);
kframes *add_gram_table(kframes * kf,char * ngram,freq * fre,index_table** it);
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
int hash_gram(char * ngram);
