#ifndef VAR
#define VAR
#include "libraries.h"
#endif

#define C2 4
extern int table_ngram_size;

typedef struct kframes{
int capacity; 	//posa xwrane
int occupied; 	//posa exoume mesa
char ** ngrams; //deiktis se pinaka me ta ngrams
int  *  k; 		//pinakas me counter emfanisewn gia ta top k
int   q; 		//counter gia to posa q exoun perasaei. diladi poses allages grammis anamesa se 2 "F"
int * ends;		//pinakas me counter gia to meta apo poses lekseis allazei to guery
int ngrams_to_free;
}kframes;

#ifndef STACK_H
#define STACK_H
#include "stack.h"
#endif


typedef struct freq{
	int * frequency;
	int * ngram;
	int unique;
}freq;

typedef struct bucket{
	int number_of_children;
	int capacity;
	int *children;
}bucket;

typedef struct hashtable{
	int total_frames;
	int bucket_to_split;
	int split_round;
	bucket *buckets;
	float load_factor;
	int bucket_capacity;
	int number_of_buckets;
	int buckets_to_free;
}hashtable;

typedef struct topk{
	kframes * kf;
	freq * fr;
	hashtable *hash_table;
}topk;

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
int  hash_gram(hashtable *hash_table, char *word);

void swap(int a, int b);

topk * create_top(topk * top);

topk *extend_top(topk * top);

topk *add_top(topk * top,char * ngram);

topk *init_top(topk * top);

topk *erase_top(topk * top);

void print_print(topk * top);

void print_top(topk * top,int k);

topk *sort_frequencies(topk * top);

topk *  end_gram_table(topk * top,int ngrams_found);

topk *increase_frequency(topk * top,char * ngram);

void print_frequencies(topk*top);

void print_hashtable(topk*top);

int partition( int *frequency,int *ngram, int l, int r,char **ngrams);

void quickSort( int *frequency,int *ngram, int l, int r,char **ngrams);
int resize_hash_for_top(hashtable *hash_,kframes *kf,freq *fr);
void shrink_buckets_for_top(bucket *bucket_,stack *stack_);
