#define M 8192 //size of  bitvector bloomfilter in bits, should be good for ut to 8000 words
#include <stdint.h>


void SetBit(int *A, int k);
void ClearBit(int *A,int k);
int TestBit(int *A,int k);
int TestAllBits(int *bloom);
void bloomfilter_init(int * bloom);
//unsigned long hash( char *str,int key);
int bloomfilter_add(char * message,int *bloom);
int bloomfilter_check(char * message,int *bloom);
void hash2(const void *in_string, int *ptr,int key);
unsigned long hash( char *str,int key);
int mymod(char * input,int mod);


