#include "bloomfilter.h"

#include <stdio.h>
#include <stdlib.h>

void SetBit(int *A, int k){
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

int TestAllBits(int *bloom){
	int i;
	int a=M;
	a/=8;
	a/=sizeof(int);
	for (i=0;i<M;i++){
		if(TestBit(bloom,i)!=0) {printf("Problem Houston");
			return 1; }//paizei kai na uparxei
	}
return 0; //sigoura den uparxei
}





void bloomfilter_init(int * bloom){
	/*int i;
	printf("size ison %lu\n",sizeof(bloom));
	//for(i=0;i<((M/sizeof(int))/8);i++){
for (i=0;i<M;i++){
		ClearBit(bloom,i);
		if(TestBit(bloom,i)==0)
			printf(GREEN"Good\n"RESET);
		else{
			printf(RED"Bad\n"RESET);
			
		}
	}
*/
//working 1
/*
	int a=M;
	a/=8;
	a/=sizeof(int);
memset(bloom,0,a);
if(TestAllBits(bloom)==0)	
			printf(BLUE"Freat Job %d %lu\n"RESET,a,sizeof(int));
		else printf("Crap\n");
*/
//end 1

// not working 2
/*
int a = ((M/sizeof(int))/8);//cells in a int table
int j;
for(j=0;j<a;j++)
	bloom[j] &=0;

*/
//end 2

//not working 3
/*
	//for(i=0;i<((M/sizeof(int))/8);i++){
int i;
	for (i=0;i<M;i++){
		ClearBit(bloom,i);
		if(TestBit(bloom,i)!=0)
			printf(GREEN"Good\n"RESET);
		else
			{
			printf(RED"Bad\n"RESET);
			
			}
}
*/
//end 3
//wornking 4

//int i;
//for (i=0;i<M;i++){
//ClearBit(bloom,i);
//}
memset(bloom,0,M/8);

//TestAllBits(bloom);
//end 4
}


unsigned long hash( char *str,int key){
    unsigned long hash=0;

	switch(key){
		case 1 :
			hash = 5381;		
			break;
		case 2 :
			hash = 8377;			
			break;
		case 3 :
			hash = 6607;			
			break;
		case 4 :
			hash = 10061;			
			break;
		case 5 :
			hash = 9133;			
			break;
		case 6 :
			hash = 5981;			
			break;
		case 7 :
			hash = 3163;			
			break;
		case 8 :
			hash = 7127;			
			break;
	}
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//printf("hash return; %lu\n",hash%M);
    return hash%M;
}

/*
int ModuloByDigits(int previousValue, int modulo)
    {
        return ((previousValue * 10) % modulo);
    }
    

void mod(char * input){
        int modulo = 97;
//        char * input = "100020778788920323232343433";
        int result = 0;
        int lastRowValue = 1;
		char c;
        for (int i = 0; i < strlen(input); i++){
		
            if (i > 0){
                lastRowValue = ModuloByDigits(lastRowValue, modulo);
            }
			c = input[i];
            result += lastRowValue * (c-48);
        }
        result = result % modulo;
		printf("Input : %s\n",input);
        printf("Result: %d\n", result);
}*/
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


void hash2(const void *in_string, int *ptr, int key){

	uint64_t * hash =  malloc(2*sizeof(uint64_t));

	MurmurHash3_x64_128(in_string,strlen(in_string),key,hash);
	//uint64_t var = hash[0]%M;
//	printf("Sofina Lazaraki%lu\n",var);
	ptr[0] = hash[0]%M;
	ptr[1] = hash[1]%M;
free(hash);
	//printf("Sofina %d %d\n",ptr[0],ptr[1]);

}



void bloomfilter_add(char * message,int *bloom){
	int *hashvalue1 = malloc(2*sizeof(int));
	int *hashvalue2 = malloc(2*sizeof(int));
	int *hashvalue3 = malloc(2*sizeof(int));

		hash2(message,hashvalue1,16);
		hash2(message,hashvalue2,32);
		hash2(message,hashvalue3,64);

		SetBit(bloom,hashvalue1[0]);
		SetBit(bloom,hashvalue1[1]);

		SetBit(bloom,hashvalue2[0]);
		SetBit(bloom,hashvalue2[1]);

		SetBit(bloom,hashvalue3[0]);
		SetBit(bloom,hashvalue3[1]);

	free(hashvalue1);
	free(hashvalue2);
	free(hashvalue3);
//return 0;
}


int bloomfilter_check(char * message,int *bloom){
	int *hashvalue1 = malloc(2*sizeof(uint64_t));
	int *hashvalue2 = malloc(2*sizeof(uint64_t));
	int *hashvalue3 = malloc(2*sizeof(uint64_t));

		hash2(message,hashvalue1,16);
		hash2(message,hashvalue2,32);
		hash2(message,hashvalue3,64);

		if((TestBit(bloom,hashvalue1[0])==0)
		  ||(TestBit(bloom,hashvalue1[1])==0)
		  ||(TestBit(bloom,hashvalue2[0])==0)
		  ||(TestBit(bloom,hashvalue2[1])==0)
		  ||(TestBit(bloom,hashvalue3[0])==0)
		  ||(TestBit(bloom,hashvalue3[1])==0))
		{
	//		printf(RED"The string is not storred\n"RESET);
			free(hashvalue1);
			free(hashvalue2);
			free(hashvalue3);
			return 0;
		}
	//returns 0 if there is one bit 0, or 1 if all of them are 1.	
	free(hashvalue1);
	free(hashvalue2);
	free(hashvalue3);
//printf(YELLOW"The string may be storred\n"RESET);
return 1;
}

