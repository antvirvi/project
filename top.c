#include "top.h"

 int table_ngram_size = 20;
/*

typ
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


*/

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


void swap(int a, int b){
	int tmp = a;
	a = b;
	b = tmp;
}

//__________________________________________ngram table
topk *  create_top(topk * top){
	top->kf = malloc(sizeof(kframes));
	top->kf->ngrams = malloc(table_ngram_size*sizeof(char *));
	top->kf->k = malloc(table_ngram_size*sizeof(int));
	top->kf->capacity = table_ngram_size;
	top->kf->occupied = 0;
	top->kf->q = 0;
	top->kf->ends = malloc(sizeof(int*));

	top->fr = malloc(table_ngram_size*(sizeof(freq)));
	top->fr->frequency = malloc(table_ngram_size*sizeof(int));
	top->fr->ngram = malloc(table_ngram_size*sizeof(int));

	int i;
	top->it = malloc(27*sizeof(topk **));	

	for(i=0;i<27;i++){
		top->it[i] = malloc(sizeof(topk *));
		top->it[i]->fr_index = malloc(table_ngram_size*sizeof(int));
		top->it[i]->ngram_index = malloc(table_ngram_size*sizeof(int));
		}
return top;
}

topk *  init_top(topk* top){
	int i;
	for(i=0;i<top->kf->capacity;i++){
		if(i<top->kf->occupied)
			free(top->kf->ngrams[i]);
		top->fr->frequency[i] = -1;
		top->fr->ngram[i] = -1;
	}

	int j;
	for(i=0;i<27;i++){
		for(j=0;j<table_ngram_size;j++){

			top->it[i]->fr_index[j] = -1;
			top->it[i]->ngram_index[j] = -1;
		}
	}
	top->kf->occupied = 0;
	top->kf->q = 0;
	return top;
	}


topk *  extend_top(topk * top){
	table_ngram_size*=2;

	top->kf->capacity = table_ngram_size;
	top->kf->ngrams = (char **)realloc(top->kf->ngrams,top->kf->capacity*sizeof(char *));
	top->kf->k = realloc(top->kf->k,top->kf->capacity*sizeof(int));

	top->fr = realloc(top->fr,table_ngram_size*(sizeof(int)));

	int i;
	for(i=0;i<27;i++){
		top->it[i]->fr_index = realloc(top->it[i]->fr_index,table_ngram_size*sizeof(int));
		top->it[i]->ngram_index = realloc(top->it[i]->ngram_index,table_ngram_size*sizeof(int));
	}
//	top = init_top(top);
	return top;
}

topk * add_top(topk * top,char * ngram){ //prosthiki enos n gram stous pinakes
	if(top->kf->occupied==(top->kf->capacity)){
		extend_top(top);
		init_top(top);
	}
	top->kf->ngrams[top->kf->occupied] = malloc((strlen(ngram)+1)*sizeof(char));
	strcpy(top->kf->ngrams[top->kf->occupied],ngram);

	int i = 0;
	while(top->fr->frequency[i] != -1)
		i++;
	top->fr->frequency[i] = 1;
	top->fr->ngram[i] = top->kf->occupied;

	
	int j;
	while(top->it[hash_gram(ngram)]->fr_index[j]!=-1)
		j++;

	top->it[hash_gram(ngram)]->fr_index[j]= i ;
	top->it[hash_gram(ngram)]->ngram_index[j] = top->kf->occupied;
	
//	print_hashtable(top);
//	printf(YELLOW"Report: ngram: %s\nFrequency: %d\nPosition: %d\nngram: %s\n\n"RESET,ngram,top->fr->frequency[i],top->fr->ngram[i],top->kf->ngrams[top->fr->ngram[i]]);
	top->kf->occupied++;
	return top;
}

topk *  erase_top(topk * top){
	int i;
	for(i=0;i<27;i++){
		free(top->it[i]->fr_index);
		free(top->it[i]->ngram_index);
		free(top->it[i]);
	}

	free(top->fr->frequency);
	free(top->fr->ngram);

	free(top->kf->ngrams);
	free(top->kf->ends);
	free(top->kf->k);  
	free(top->kf);
	return top;
}

void print_print(topk * top){ //ektypwnei ola ta ngrams me
	int i;
	int j=0;
	for(i=0;i<top->kf->occupied;i++){
		printf("%s",top->kf->ngrams[i]);  //ean ftasoume sto telos enos q prepei na orisoume tin allagi gramis kai to oxi "|"
		if(top->kf->ends[j]==i){
			printf("\n");
			j++;
		}
		else
			printf("|");
	}
}

void print_top(topk*top,int k){ //ektypwnei ola ta ngrams me
	int i;
	printf("Top: ");
	for(i=0;i<k;i++){
		printf("%s",top->kf->ngrams[top->fr->ngram[i]]);
		if(i==k-1)	
			break;
		printf("|");
		}
printf("\n");
}

void print_frequencies(topk*top){ //ektypwnei ola ta ngrams me
	int i;
	for(i=0;i<top->kf->capacity;i++){
		printf("Freq %d %d\n",top->fr->frequency[i],top->fr->ngram[i]);
		}

}

void print_hashtable(topk*top){ //ektypwnei ola ta ngrams me
	int i;
	for(i=0;i<27;i++){
		printf("%c_______________\n",'A'+i);
		int j=0;
		while(top->it[i]->fr_index[j]!=-1){
			printf("It %d %d\n",top->it[i]->fr_index[j],top->it[i]->ngram_index[j]);
		}
	}

}

//stoped here
topk *  sort_frequencies(topk* top){

	int i;
	int flag = 1;
	while(flag){
		flag = 0;
		for(i=0;i<top->kf->occupied-1;i++){
			if(top->fr->frequency[i]<top->fr->frequency[i+1]){
				swap(top->fr->frequency[i],top->fr->frequency[i+1]);
				swap(top->fr->ngram[i],top->fr->ngram[i+1]);
				flag = 1;
			}
		}
	}
	return top;
}

topk * increase_frequency(topk* top, char * ngram){
	int a = hash_gram(ngram);
	int j = 0;
	while((top->it[a]->fr_index[j]!=-1)&&(j<top->kf->occupied)){
		if(strcmp(top->kf->ngrams[top->it[a]->ngram_index[j]],ngram)==0){
			top->fr->frequency[top->it[a]->fr_index[j]]++;
			return top;
		}
	j++;
	}printf(RED"Frequency not updated\n"RESET);
	return top;
}

topk *  end_gram_table(topk * top){ //simeiwnoume oti edw teleiwnei to Q, ara prepei stin ektypwsi na valoume allagi grammis
	top->kf->ends[top->kf->q] = top->kf->occupied-1;
	top->kf->ends = realloc(top->kf->ends,((top->kf->q)+1)*(sizeof(int*)));
	top->kf->q++;
	top->kf->ends[top->kf->q] = -1;
	return top;
}
