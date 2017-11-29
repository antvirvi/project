#include "top.h"

 int table_ngram_size = 2;
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

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"


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
	printf("Extend1 %d\n",table_ngram_size);
	table_ngram_size*=2;
	kf->capacity = table_ngram_size;
	printf("Extend2 %d\n",table_ngram_size);
	kf->ngrams = (char **)realloc(kf->ngrams,kf->capacity*sizeof(char *));
	kf->k = realloc(kf->k,kf->capacity*sizeof(int));
//	kf->capacity = table_ngram_size;

return kf;
}

kframes *init_gram_table(kframes * kf){

	kf->occupied = 0;
	kf->q = 0;

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
//	printf("End gram table %d %d\n",kf->q,kf->ends[kf->q]);
//	printf("test end gram%d\n",kf->ends[kf->q]);
	kf->ends[kf->q] = kf->occupied;
	kf->ends = realloc(kf->ends,((kf->q)+1)*(sizeof(int*)));
//	printf("End gram table Q:%d end:%d\n",kf->q,kf->ends[kf->q]);
	kf->q++;
	kf->ends[kf->q] = -1;

}

void erase_gram_table(kframes * kf){
	int i;
	for(i=0;i<kf->capacity;i++){
		free(kf->ngrams[i]);
	printf("freed %d\n",i);
	}
	free(kf->ngrams);
	free(kf->ends);
	free(kf->k);  
	free(kf);
}



//table holding the top appearances
void create_top_table(){


}



