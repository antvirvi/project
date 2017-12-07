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

*/
#define RED "\x1b[31m"


int  hash_gram(hashtable *hash_table, char *word)
{	
	int hash_value;
    unsigned long hash_int = 5381;
    int c;

    while (c = *word++)
        hash_int = ((hash_int << 5) + hash_int) + c; /* hash * 33 + c */
	
	hash_value=hash_int%(C2<<hash_table->split_round);

	int temp=hash_int%(C2<<(hash_table->split_round+1));

	if(temp<hash_table->number_of_buckets) return temp;

    return hash_value;
}


void swap(int a, int b){
	int tmp = a;
	a = b;
	b = tmp;
}

//__________________________________________ngram table
topk *  create_top(topk * top){
	int i;
	top =malloc(sizeof(topk));
	top->kf = malloc(sizeof(kframes));
	top->kf->ngrams = malloc(table_ngram_size*sizeof(char *));
	top->kf->k = malloc(table_ngram_size*sizeof(int));
	top->kf->capacity = table_ngram_size;
	top->kf->occupied = 0;
	top->kf->q = 0;
	top->kf->ends = malloc(sizeof(int));
	top->kf->ngrams_to_free=0;

	top->fr = malloc((sizeof(freq)));
	top->fr->frequency = malloc(table_ngram_size*sizeof(int));
	top->fr->ngram = malloc(table_ngram_size*sizeof(int));

	top->hash_table=malloc(sizeof(hashtable));
	top->hash_table->total_frames=0;
	top->hash_table->bucket_to_split=0;
	top->hash_table->split_round=0;
	top->hash_table->load_factor=0.9;
	top->hash_table->bucket_capacity=1;
	top->hash_table->number_of_buckets=C2;
	top->hash_table->buckets_to_free=C2;
	top->hash_table->buckets=malloc(top->hash_table->number_of_buckets*sizeof(bucket));
	bucket *hash_bucket;
	for(i=0;i<top->hash_table->number_of_buckets;i++){
		hash_bucket=&(top->hash_table->buckets[i]);
		hash_bucket->number_of_children=0;
		hash_bucket->capacity=top->hash_table->bucket_capacity;
		hash_bucket->children=malloc(hash_bucket->capacity*sizeof(int));
	}
	
	return top;
}


topk *  init_top(topk* top){
	int i;
	top->kf->occupied = 0;
	top->kf->q = 0;

	top->hash_table->total_frames=0;
	top->hash_table->bucket_to_split=0;
	top->hash_table->split_round=0;
	top->hash_table->number_of_buckets=4;
	bucket *hash_bucket;
	for(i=0;i<top->hash_table->number_of_buckets;i++){
		 hash_bucket=&(top->hash_table->buckets[i]);
		 hash_bucket->number_of_children=0;
	}
	

	return top;
	}


topk *  extend_top(topk * top){
	table_ngram_size*=2;

	top->kf->capacity = table_ngram_size;
	top->kf->ngrams = (char **)realloc(top->kf->ngrams,top->kf->capacity*sizeof(char *));
	top->kf->k = realloc(top->kf->k,top->kf->capacity*sizeof(int));

	top->fr->frequency = realloc(top->fr->frequency,table_ngram_size*sizeof(int));
	top->fr->ngram = realloc(top->fr->ngram,table_ngram_size*sizeof(int));

	return top;
}

topk * add_top(topk * top,char * ngram){ //prosthiki enos n gram stous pinakes
	
	if(top->kf->occupied==(top->kf->capacity)){
		extend_top(top);
	}
	//printf("in add to top\n");
	int ngram_len=strlen(ngram)+1;
	int last_position=top->kf->occupied;

	if(last_position+1>top->kf->ngrams_to_free){
		top->kf->ngrams[last_position] = malloc(ngram_len*sizeof(char));
		top->kf->ngrams_to_free+=1;
	}
	else{ 
		top->kf->ngrams[last_position] = realloc(top->kf->ngrams[last_position],ngram_len*sizeof(char));
	}

	memmove(top->kf->ngrams[last_position],ngram,ngram_len);


	top->fr->frequency[last_position] = 1;
	top->fr->ngram[last_position] =last_position;
	top->kf->occupied++;
	
	
	//add in hash table if its new
	hashtable *hash_=top->hash_table;

	if((hash_->total_frames/((float)hash_->number_of_buckets*hash_->bucket_capacity)) > hash_->load_factor){
		int resize_error=resize_hash_for_top(hash_,top->kf);
		if(resize_error==-1) return NULL;
	}	
	
	int j=0;
	int hash_value=hash_gram(top->hash_table,ngram);
	int last_in_bucket,pos;
	//check if exists in bucket first
	bucket *hash_bucket=&(top->hash_table->buckets[hash_value]);
	last_in_bucket=hash_bucket->number_of_children;

	/*for(j=0;j<last_in_bucket;j++){
		pos=hash_bucket->children[j];
		if(strcmp(top->kf->ngrams[pos],ngram)==0)
		{
			//increase ngram frequency
			top->fr->frequency[pos]++;
		}
		return;
	}
	top->fr->frequency[last_position] = 1;
	//
	
	if((hash_->total_frames/((float)hash_->number_of_buckets*hash_->bucket_capacity)) > hash_->load_factor){
		int resize_error=resize_hash_for_top(hash_,top->kf);
		if(resize_error==-1) return NULL;
	}	
	*/
	if(last_in_bucket==hash_bucket->capacity){ 	//overflow bucket
		hash_bucket->children=realloc(hash_bucket->children,hash_bucket->capacity*2*sizeof(int));
		hash_bucket->capacity*=2;
	}

	hash_bucket->children[last_in_bucket]=last_position;
	top->hash_table->total_frames++;
	hash_bucket->number_of_children++;

	return top;
}

void initialize_bucket_for_top(bucket *bucket_,int m){
	bucket_->children=malloc(m*sizeof(int));
	bucket_->number_of_children=0;
	bucket_->capacity=m;
}

int resize_hash_for_top(hashtable *hash_,kframes *kf){
	bucket *hash_bucket;
	//printf("free buckets %d , used %d\n",hash_->buckets_to_free,hash_->number_of_buckets);
	if(hash_->buckets_to_free==hash_->number_of_buckets){
		//printf("WILL ADD MORE\n");
		hash_->buckets=realloc(hash_->buckets,(hash_->number_of_buckets+1)*sizeof(bucket)); //add bucket lineat
		if(hash_->buckets==NULL){
			//printf("error in realloc\n");
			return -1;
		}
		initialize_bucket_for_top(&(hash_->buckets[hash_->number_of_buckets]),hash_->bucket_capacity);
		hash_->buckets_to_free++;
	}
	hash_->number_of_buckets++;
	
	int i;
	bucket *new_bucket=&(hash_->buckets[hash_->number_of_buckets-1]); //pointer to the new bucket
	new_bucket->number_of_children=0;
	new_bucket->capacity=hash_->bucket_capacity;

	hash_bucket=&(hash_->buckets[hash_->bucket_to_split]); //re arranging bucket to split
	int new_hash_val=-1;
	stack *stack_=init_stack();
	//print_stack(stack_);

	if(hash_bucket->number_of_children==0){
		hash_->bucket_to_split=(hash_->bucket_to_split+1)%(C2<<hash_->split_round);//without pow
		if(hash_->bucket_to_split==0 && hash_->number_of_buckets>C2) hash_->split_round++;
		stack_destroy(stack_); 
		return 1;
	} // no need for rearranging bucket
	
	int previous=hash_->bucket_to_split;
	hash_->bucket_to_split=(hash_->bucket_to_split+1)%(C2<<hash_->split_round);// without pow
	int pos;
	for(i=0;i<hash_bucket->number_of_children;i++){
		pos=hash_bucket->children[i];
		new_hash_val=hash_gram(hash_,kf->ngrams[pos]);

		if(new_hash_val==previous) continue; //if hash value is the same then no need to change bucket
		if(new_bucket->number_of_children==new_bucket->capacity){	//if new bucket fills then create an overflow bucket
			new_bucket->children=realloc(new_bucket->children,hash_bucket->capacity*2*sizeof(int));
			if(new_bucket->children==NULL){
				stack_destroy(stack_); 
				return -1;
			}
			new_bucket->capacity=new_bucket->capacity*2;	
		}
		memmove(&(new_bucket->children[new_bucket->number_of_children]),&(hash_bucket->children[i]),sizeof(int)); //copy nod
		push(stack_,i);		
		new_bucket->number_of_children++;	
	}

	shrink_buckets_for_top(&(hash_->buckets[previous]),stack_);
	if(hash_->bucket_to_split==0 && hash_->number_of_buckets>C2) hash_->split_round++;
	stack_destroy(stack_); 
	return 1;
}


void shrink_buckets_for_top(bucket *bucket_,stack *stack_){
	int i;
	int pos;
	int *total=&(bucket_->number_of_children);
	for(i=stack_->top-1;i>=0;i--){
		pos=get_stack_elements(stack_,i);
		
		memmove(&(bucket_->children[pos]),&(bucket_->children[pos+1]),(*total-(pos+1))*sizeof(int));
		*total=*total-1;
		}
}

topk *  erase_top(topk * top){
	int i;
	bucket *hash_bucket;
	for(i=0;i<top->hash_table->buckets_to_free;i++){
		hash_bucket=&(top->hash_table->buckets[i]);
		free(hash_bucket->children);
	}
	free(top->hash_table->buckets);
	free(top->hash_table);

	free(top->fr->frequency);
	free(top->fr->ngram);
	free(top->fr);
	//printf("to free are %d\n",top->kf->ngrams_to_free);
	for(i=0;i<top->kf->ngrams_to_free;i++) free(top->kf->ngrams[i]);

	free(top->kf->ngrams);
	free(top->kf->ends);
	free(top->kf->k);  
	free(top->kf);
	free(top);
	return top;
}

void print_print(topk * top){ //ektypwnei ola ta ngrams me
	int i;
	int j=0;
	//printf("occupied are %d\n",top->kf->occupied);
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
	int max=k;
	//printf("occupied are %d\n",top->kf->occupied);
	if(top->kf->occupied<max) max=top->kf->occupied;
	if(max==0) return;
	//print_frequencies(top);
	quickSort(top->fr->frequency,top->fr->ngram,0,max-1,top->kf->ngrams);
	//print_frequencies(top);
	printf("Top: ");
	for(i=0;i<max;i++){
		printf("%s",top->kf->ngrams[top->fr->ngram[i]]);
		if(i==k-1)	
			break;
		printf("|");
		}
printf("\n");
}

void print_frequencies(topk*top){ //ektypwnei ola ta ngrams me
	int i;
	for(i=0;i<top->kf->occupied;i++){
		printf("Freq %d %d\n",top->fr->frequency[i],top->fr->ngram[i]);
		}

}

void print_hashtable(topk*top){ //ektypwnei ola ta ngrams me
	

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

void quickSort( int *frequency,int *ngram, int l, int r,char **ngrams)
{
	int j;
	if(l<r){
   	// divide and conquer
		j = partition(frequency,ngram,l,r,ngrams);
     	quickSort(frequency,ngram,l,j-1,ngrams);
     	quickSort(frequency,ngram,j+1,r,ngrams);
	}
}



/*int partition( int *frequency,int *ngram, int l, int r,char ** ngrams) {
   int pivot,i,j,temp;

   pivot = frequency[l]; //first element as the pivot
   i = l; j = r+1;
		
	while(1)
	{
   		do{
		 ++i;
		}
		while( i <= r && frequency[i] >= pivot );
   		do --j; while( frequency[j] < pivot );
   		if( i >= j ) break;
   		temp = frequency[i];
		frequency[i] = frequency[j];
		frequency[j] = temp; //swapping i,j

		temp = ngram[i];
		ngram[i] = ngram[j];
		ngram[j] = temp; 
	}
	temp = frequency[l];
	frequency[l] = frequency[j];
	frequency[j] = temp;

	temp = ngram[l];
	ngram[l] = ngram[j];
	ngram[j] = temp; 
	return j;
}*/

int partition (int *frequency,int *ngram,int l,int r,char **ngrams){
    // pivot (Element to be placed at left position)
	int i,j,temp;
    int pivot = frequency[r];
    i =(l-1);  // Index of smaller element
	int compare;
    for (j=l;j<=r-1;j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (frequency[j]>=pivot)
        {
            i++;    // increment index of smaller element
			temp = frequency[i];
			frequency[i] = frequency[j];
			frequency[j] = temp;

			temp = ngram[i];
			ngram[i] = ngram[j];
			ngram[j] = temp; 
        }
		else if(frequency[j]==pivot){	
			compare=strcmp(ngrams[j],ngrams[pivot]); //change ngrams based on alphabet
			if(compare>0){
				i++;    // increment index of smaller element
				temp = frequency[i];
				frequency[i] = frequency[j];
				frequency[j] = temp;
				
				temp = ngram[i];
				ngram[i] = ngram[j];
				ngram[j] = temp; 
			
			}
		}
    }
	
	temp = frequency[i+1];
	frequency[i+1] = frequency[r];
	frequency[r] = temp;

	temp = ngram[i+1];
	ngram[i+1] = ngram[r];
	ngram[r] = temp;

    return (i+1);
}


topk * increase_frequency(topk* top, char * ngram){
	int hash_value = hash_gram(top->hash_table,ngram);
	int i = 0;
	int position;
	bucket *hash_bucket=&(top->hash_table->buckets[hash_value]);
	for(i=0;i<hash_bucket->number_of_children;i++){
		position=hash_bucket->children[i];
		if(strcmp(top->kf->ngrams[position],ngram)==0){
			top->fr->frequency[position]++;
			return top;
		}
	}
	
	printf(RED"Frequency not updated\n"RESET);
	return top;
}

topk *  end_gram_table(topk * top){ //simeiwnoume oti edw teleiwnei to Q, ara prepei stin ektypwsi na valoume allagi grammis
	top->kf->ends[top->kf->q] = top->kf->occupied-1;
	top->kf->ends = realloc(top->kf->ends,((top->kf->q)+1)*(sizeof(int*)));
	top->kf->q++;
	top->kf->ends[top->kf->q] = -1;
	return top;
}

