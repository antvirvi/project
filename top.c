#include "top.h"

 int table_ngram_size = 20;
/*

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
	int fr_index;
	int ngram;
	struct index_table * next;
}index_table;

*/
//table holding all the ngramms

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"



int hash_gram(char * ngram){ //epistrefei ena noumero gia kathe rpwto gramma eos ngram
if((ngram[0]>='a')&&(ngram[0]<='z'))
	return ngram[0]-'a';
else if((ngram[0]>='A')&&(ngram[0]<='Z'))
	return ngram[0]-'A';
else if((ngram[0]>='0')&&(ngram[0]<='9'))
	return 26;
else{ 
	printf("Something strange in hash_gram %s\n",ngram);
	return -1;
	}
}
//__________________________________________ngram table

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

kframes *extend_gram_table(kframes * kf,freq * fre,index_table** it){
	table_ngram_size*=2;
	kf->capacity = table_ngram_size;
	kf->ngrams = (char **)realloc(kf->ngrams,kf->capacity*sizeof(char *));
	kf->k = realloc(kf->k,kf->capacity*sizeof(int));
	fre = realloc(fre,table_ngram_size*(sizeof(int)));
//	kf->capacity = table_ngram_size;

return kf;
}

kframes *add_gram_table(kframes * kf,char * ngram,freq * fre,index_table** it){ //prosthiki enos n gram stonpinaka
	if(kf->occupied==(kf->capacity)){
		kf = extend_gram_table(kf,fre,it);
	}
//	printf("cap %d occ %d ngr %s \n\n\n_\n",kf->capacity,kf->occupied,ngram);
	kf->ngrams[kf->occupied] = malloc((strlen(ngram)+1)*sizeof(char));
	strcpy(kf->ngrams[kf->occupied],ngram);
	fre->frequency++;
	fre->ngram = kf->occupied;
	kf->occupied++;
	
	
return kf;
}

kframes *init_gram_table(kframes * kf){
	int i ;
	for(i=0;i<kf->occupied;i++){
		free(kf->ngrams[i]);
	}
	kf->occupied = 0;
	kf->q = 0;

return kf;
}

void erase_gram_table(kframes * kf){
	free(kf->ngrams);
	free(kf->ends);
	free(kf->k);  
	free(kf);
}

void print_gram_table(kframes *kf){ //ektypwnei ola ta ngrams me
	int i;
	int j=0;
	//int * ptr;
//	ptr = kf->ends;
	for(i=0;i<kf->occupied;i++){ 
		printf("%s",kf->ngrams[i]);  //ean ftasoume sto telos enos q prepei na orisoume tin allagi gramis kai to oxi "|"
		if(kf->ends[j]==i){
			printf("\n");
			j++;
		}
		else
			printf("|");
	}
}

void end_gram_table(kframes *kf){ //simeiwnoume oti edw teleiwnei to Q, ara prepei stin ektypwsi na valoume allagi grammis
	kf->ends[kf->q] = kf->occupied-1;
	kf->ends = realloc(kf->ends,((kf->q)+1)*(sizeof(int*)));
	kf->q++;
	kf->ends[kf->q] = -1;

}



//__________________________________________frequency
void create_freq_index(freq * fr){
	fr = malloc(table_ngram_size*(sizeof(freq)));
	fr->frequency = malloc(table_ngram_size*sizeof(int));
	int i;
	for(i=0;i<table_ngram_size-1;i++){
		fr->frequency[i] = 0;
		}
	
}

void init_freq_index(freq* fr){
int i;
	for(i=0;i<table_ngram_size-1;i++){
		fr->frequency[i] = 0;
		}

}

void erase_freq_index(freq* fr){
		free(fr->frequency);

}


void update_frequency(char * ngram){

	//int a = hash_gram(ngram);

}


//__________________________________________hash table

void create_hash_index(index_table** it){
	int i;
	it = malloc(27*sizeof(struct index_table *));
	for(i=0;i<27;i++){
		it[i] = malloc(sizeof(index_table));
		it[i]= NULL;
	}
}

void init_hash_index(index_table* it){


}

void insert_hash_index(char * ngram, index_table * it){
//int i = hash_gram(ngram);

//if(it[i]==NULL)
//	strcpy(it[i]->ngram
//
}
