#include "bloomfilter.h"

#include <stdio.h>
#include <stdlib.h>

int bloomfiltersize(int input_size){
	float p = 0.01; //fault probability. 
	int n = input_size; //size of input
	float a = n*log(1/p);
	float b = log(2)*log(2);
	float ab = a/b;
	int abc = ab;
	int m = ceil(abc);
	float  temp = m/64;
	m = ceil(temp);
	m *= 64;
//	printf("Panos %d\n",m);
//	return 8192;
	return m;
}

int hashfunctionssize(int input_size){
	float p = 0.01; //fault probability. 
	int k = (bloomfiltersize(input_size)/input_size)*log(2);
//	printf("Panos2 %d\n",k);
//	return 6;
	if(k>6) 
		return 6;
	return k;
}

void SetBit(int *A, int k){	// 32 is sizeof(int)
	int i = k/32;            // i = array index (use: A[i])
	int pos = k%32;          // pos = bit position in A[i]
	unsigned int flag = 1;   // flag = 0000.....00001
	flag = flag << pos;      // flag = 0000...010...000   (shifted k positions)
	A[i] = A[i] | flag;      // Set the bit at the k-th position in A[i]
}

void ClearBit(int *A,int k){
	int i = k/32;
	int pos = k%32;
	unsigned int flag = 1;  // flag = 0000.....00001
	flag = flag << pos;     // flag = 0000...010...000   (shifted k positions)
	flag = ~flag;           // flag = 1111...101..111
	A[i] = A[i] & flag;     // RESET the bit at the k-th position in A[i]
}


int TestBit(int *A,int k){
	int i = k/32;
	int pos = k%32;
	unsigned int flag = 1;  // flag = 0000.....00001
	flag = flag << pos;     // flag = 0000...010...000   (shifted k positions)
	if ( A[i] & flag )      // Test the bit at the k-th position in A[i]
		return 1;
	else
		return 0;
}

int TestAllBits(int *bloom,size_t bloom_size){
	int i;
	int a=bloom_size;
	a/=8;
	a/=sizeof(int);
	for (i=0;i<bloom_size;i++){
		if(TestBit(bloom,i)!=0) {printf("Problem Houston");
			return 1; }//paizei kai na uparxei
	}
return 0; //sigoura den uparxei
}





void bloomfilter_init(int * bloom,size_t bloom_size){
memset(bloom,0,bloom_size/8);
//TestAllBits(bloom);
}

int mymod(char * input,int mod)
{
	int a = 0;
	int len = strlen(input);
	int ret = 0;
	int c;
	int dec = 0;

	for(a=0;a<len;a++){
		if(input[a]>57)
			c = (input[a]-'a');
		else 
			c = (input[a]-'0');
		
		ret += (c*16);
		ret = ret % mod;
		dec +=(c*16);
printf("%d %s %d %d\n",ret, input, dec, mod);
	}

return ret;
}


void hash2(const void *in_string, int *ptr, int key,size_t bloom_size){

	uint64_t * hash =  malloc(2*sizeof(uint64_t));

	MurmurHash3_x64_128(in_string,strlen(in_string),key,hash);
	//uint64_t var = hash[0]%M;
	ptr[0] = hash[0]%bloom_size;
	ptr[1] = hash[1]%bloom_size;
	free(hash);


}



void bloomfilter_add(char * message,int *bloom,size_t bloom_size){

	int lim = hashfunctionssize(bloom_size);

	int *hashvalue1 = malloc(6*sizeof(int));
//	int *hashvalue2 = malloc(2*sizeof(int));
//	int *hashvalue3 = malloc(2*sizeof(int));

		hash2(message,hashvalue1,16,bloom_size);
		hash2(message,hashvalue1+2,32,bloom_size);
		hash2(message,hashvalue1+4,64,bloom_size);


	switch(lim){
		case 6 :
			SetBit(bloom,hashvalue1[5]);
		case 5	:
			SetBit(bloom,hashvalue1[4]);
		case 4	:
			SetBit(bloom,hashvalue1[3]);
		case 3	:
			SetBit(bloom,hashvalue1[2]);
		case 2	:
			SetBit(bloom,hashvalue1[1]);
		case 1	:
			SetBit(bloom,hashvalue1[0]);
	}

//	free(hashvalue3);
//	free(hashvalue2);
	free(hashvalue1);


/*
	for (i=1;i<=lim/2;i++){
			hash2(message,hashvalue,i*2,bloom_size);
			SetBit(bloom,hashvalue[0]);
			SetBit(bloom,hashvalue[1]);
	}
	if(lim%2){
		hash2(message,hashvalue,i*2,bloom_size);
		SetBit(bloom,hashvalue[0]);
	}
	free(hashvalue);*/
}


int bloomfilter_check(char * message,int *bloom,size_t bloom_size){

/*
	int *hashvalue = malloc(2*sizeof(int));
	int lim = hashfunctionssize(bloom_size);
	int i;
	for (i=1;i<=lim/2;i++){
			hash2(message,hashvalue,i*2,bloom_size);
			if((TestBit(bloom,hashvalue[0])==0)||(TestBit(bloom,hashvalue[1])==0))
				return 0;
	}
	if(lim%2){
		hash2(message,hashvalue,i*2,bloom_size);
		if(TestBit(bloom,hashvalue[0])==0)
			return 0;
	}
	free(hashvalue);
return 1;
*/
	int lim = hashfunctionssize(bloom_size);
	int *hashvalue1 = malloc(6*sizeof(int));
//	int *hashvalue2 = malloc(2*sizeof(int));
//	int *hashvalue3 = malloc(2*sizeof(int));

		hash2(message,hashvalue1+0,16,bloom_size);
		hash2(message,hashvalue1+2,32,bloom_size);
		hash2(message,hashvalue1+4,64,bloom_size);


	switch(lim){
		case 6 :
			if(TestBit(bloom,hashvalue1[5])==0){
//				free(hashvalue3);
//				free(hashvalue2);
				free(hashvalue1);
				return 0;
			}
				
		case 5	:
			if(TestBit(bloom,hashvalue1[4])==0){
//				free(hashvalue3);
//				free(hashvalue2);
				free(hashvalue1);
				return 0;
			}
		case 4	:
			if(TestBit(bloom,hashvalue1[3])==0){
//				free(hashvalue3);
//				free(hashvalue2);
				free(hashvalue1);
				return 0;
			}
		case 3	:
			if(TestBit(bloom,hashvalue1[2])==0){
//				free(hashvalue3);
//				free(hashvalue2);
				free(hashvalue1);
				return 0;
			}
		case 2	:
			if(TestBit(bloom,hashvalue1[1])==0){
//				free(hashvalue3);
//				free(hashvalue2);
				free(hashvalue1);
				return 0;
			}
		case 1	:
			if(TestBit(bloom,hashvalue1[0])==0){
//				free(hashvalue3);
//				free(hashvalue2);
				free(hashvalue1);
				return 0;
			}
	}

//	free(hashvalue3);
//	free(hashvalue2);
	free(hashvalue1);

}

