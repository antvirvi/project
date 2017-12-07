#include "top.h"

 int table_ngram_size = 20;
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
	kf->capacity = table_ngram_size;
	kf->ngrams = (char **)realloc(kf->ngrams,kf->capacity*sizeof(char *));
	kf->k = realloc(kf->k,kf->capacity*sizeof(int));
//	kf->capacity = table_ngram_size;

return kf;
}

kframes *add_gram_table(kframes * kf,char * ngram){ //prosthiki enos n gram stonpinaka
	if(kf->occupied==(kf->capacity))
		kf = extend_gram_table(kf);
//	printf("cap %d occ %d ngr %s \n\n\n_\n",kf->capacity,kf->occupied,ngram);
	kf->ngrams[kf->occupied] = malloc((strlen(ngram)+1)*sizeof(char));
	strcpy(kf->ngrams[kf->occupied],ngram);
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
	
	//printf("PRINT ends with kf->q %d\n",kf->q);
	//for(i=0;i<kf->q;i++) {
		//printf("%d - ",kf->ends[i]);
	//}
	//printf("\n");
	
	for(i=0;i<kf->occupied;i++){ 
		printf("%s",kf->ngrams[i]);  //ean ftasoume sto telos enos q prepei na orisoume tin allagi gramis kai to oxi "|"
		//printf("ends is %d with j %d\n",kf->ends[j],j);		
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

//table holding the top appearances
void create_top_table(){


}



