//#define M 8192 //size of  bitvector bloomfilter in bits, should be good for ut to 8000 words
#include <stdint.h>
#ifndef VAR
#define VAR
#include "libraries.h"
#endif
#include "murmur3.h"
extern int M;

void SetBit(int *A, int k);
void ClearBit(int *A,int k);
int TestBit(int *A,int k);
//int TestAllBits(int *bloom);
void bloomfilter_init(int * bloom,size_t bloom_size);
//unsigned long hash( char *str,int key);
//void bloomfilter_add(char * message,int *bloom);
//int bloomfilter_check(char * message,int *bloom);
//void hash2(const void *in_string, int *ptr,int key);
//unsigned long hash( char *str,int key);
int mymod(char * input,int mod);

void bloomfilter_add(char * message,int *bloom,size_t bloom_size);
int bloomfilter_check(char * message,int *bloom,size_t bloom_size);
int TestAllBits(int *bloom,size_t bloom_size);
void hash2(const void *in_string, int *ptr, int key,size_t bloom_size);
unsigned long hash( char *str,int key,size_t bloom_size);
