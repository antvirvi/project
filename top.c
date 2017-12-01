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

void create_top(kframes * kf, freq * fr, index_table ** it){
	kf = malloc(sizeof(kframes));
	kf->ngrams = malloc(table_ngram_size*sizeof(char *));
	kf->k = malloc(table_ngram_size*sizeof(int));
	kf->capacity = table_ngram_size;
	kf->occupied = 0;
	kf->q = 0;
	kf->ends = malloc(sizeof(int*));

	fr = malloc(table_ngram_size*(sizeof(freq)));
	fr->frequency = malloc(table_ngram_size*sizeof(int));
	fr->ngram = malloc(table_ngram_size*sizeof(int));
	/*for(i=0;i<table_ngram_size-1;i++){
		fr->frequency[i] = -1;
		fr->ngram[i] = -1;
	}

	it = malloc(27*sizeof(struct index_table *));//26 for the letters and 1 for the numbers
	for(i=0;i<27;i++){
		it[i] = malloc(sizeof(index_table));
		it[i]= NULL;
	}
*/
}

void extend_top(kframes * kf,freq * fre,index_table** it){
	table_ngram_size*=2;
	kf->capacity = table_ngram_size;
	kf->ngrams = (char **)realloc(kf->ngrams,kf->capacity*sizeof(char *));
	kf->k = realloc(kf->k,kf->capacity*sizeof(int));

	fre = realloc(fre,table_ngram_size*(sizeof(int)));
}

void add_top(kframes * kf,char * ngram,freq * fre,index_table** it){ //prosthiki enos n gram stous pinakes
	if(kf->occupied==(kf->capacity)){
		extend_top(kf,fre,it);
	}
	kf->ngrams[kf->occupied] = malloc((strlen(ngram)+1)*sizeof(char));
	strcpy(kf->ngrams[kf->occupied],ngram);

	int i = 0;
	while(fre->frequency[i] != -1)
		i++;
	fre->frequency[i] = 1;
	fre->ngram[i] = kf->occupied;

	kf->occupied++;
	
	index_table * ptr = it[hash_gram(ngram)];
	while(ptr!=NULL)
		ptr=ptr->next;
	
	ptr->fr_index = i;
	ptr->ngram = kf->occupied-1;
	ptr->next = malloc(sizeof(index_table));
	ptr->next = NULL;
}

void init_top(kframes * kf,freq * fr, index_table** it){
	int i;
	for(i=0;i<kf->occupied;i++){
		free(kf->ngrams[i]);
		fr->frequency[i] = -1;
		fr->ngram[i] = -1;
	}

	it = malloc(27*sizeof(struct index_table *));//26 for the letters and 1 for the numbers
	for(i=0;i<27;i++){
		it[i] = malloc(sizeof(index_table));
		it[i]= NULL;
	}

	kf->occupied = 0;
	kf->q = 0;
}

void erase_top(kframes * kf,freq * fr, index_table ** it){
	int i;
	index_table * ptr, *pre_ptr;
	for(i=0;i<27;i++){
		ptr = it[i];
		while(ptr!=NULL){
			pre_ptr = ptr;
			ptr = ptr->next;
			free(pre_ptr);
		}
	free(ptr);
	}

	free(fr->frequency);
	free(fr->ngram);

	free(kf->ngrams);
	free(kf->ends);
	free(kf->k);  
	free(kf);
}

void print_print(kframes *kf){ //ektypwnei ola ta ngrams me
	int i;
	int j=0;
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

void print_top(kframes *kf,freq * fr,index_table ** it,int k){ //ektypwnei ola ta ngrams me
	int i;
	printf("Top: ");
	for(i=0;i<k;i++){
		printf("%s",kf->ngrams[fr->ngram[i]]);
		if(i==k-1)	break;
		printf("|");
		}
}
void sort_frequencies(kframes *kf, freq* fr,	index_table ** it){

	int i;
	int flag = 1;
	while(flag){
		flag = 0;
		for(i=0;i<kf->occupied-1;i++){
			if(fr->frequency[i]<fr->frequency[i+1]){
				swap(fr->frequency[i],fr->frequency[i+1]);
				swap(fr->ngram[i],fr->ngram[i+1]);
				flag = 1;
			}
		}
	}
}

void increase_frequency(char * ngram,kframes *kf, freq* fr,	index_table ** it){

	int a = hash_gram("ngram");
	index_table* ptr = it[a];
	while(strcmp(kf->ngrams[ptr->ngram],ngram)!=0){
		ptr = ptr->next;
	if(ptr==NULL){printf("Something went wrong in increase frequency\n");return;}

	}
	if(strcmp(kf->ngrams[ptr->ngram],ngram)==0){
		fr->frequency[ptr->fr_index]++;
	}
}

void end_gram_table(kframes *kf){ //simeiwnoume oti edw teleiwnei to Q, ara prepei stin ektypwsi na valoume allagi grammis
	kf->ends[kf->q] = kf->occupied-1;
	kf->ends = realloc(kf->ends,((kf->q)+1)*(sizeof(int*)));
	kf->q++;
	kf->ends[kf->q] = -1;

}
