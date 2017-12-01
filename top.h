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
	int * frequency;
	int * ngram;
}freq;


typedef struct index_table{
	int fr_index;
	int ngram;
	struct index_table * next;
}index_table;

/*
kframes *create_gram_table(kframes * kf);
kframes *extend_gram_table(kframes * kf,freq * fre,index_table** it);
kframes *init_gram_table(kframes * kf);
kframes *add_gram_table(kframes * kf,char * ngram,freq * fre,index_table** it);

kframes *newline_gram_table(kframes * kf){
	if(occupied==capacity)
		kf = extend_gram_table(kf);

	kf->ngrams[kf->occupied] = malloc(2);
	strcpy(kf->ngrams[kf->occupied],"\n");
	occupied++;

return kf;
}
*/

//__________________________________________ngram table


void swap(int a, int b);
void create_top(kframes * kf,freq * fre, index_table ** it);
void extend_top(kframes * kf,freq * fre,index_table** it);

void add_top(kframes * kf,char * ngram,freq * fre,index_table** it);

void init_top(kframes * kf,freq * fr, index_table** it);

void erase_top(kframes * kf,freq * fr, index_table ** it);

void print_print(kframes *kf);

void print_top(kframes *kf,freq * fr,index_table ** it,int k);

void sort_frequencies(kframes *kf, freq* fr,	index_table ** it);

void end_gram_table(kframes *kf);

void increase_frequency(char * ngram,kframes *kf, freq* fr,	index_table ** it);
