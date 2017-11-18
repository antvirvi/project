#include "bloomfilter.h"
#include "libraries.h"



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
		if(TestBit(bloom,i)!=0)
			return 1; //paizei kai na uparxei
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

int i;
for (i=0;i<M;i++){
ClearBit(bloom,i);
}

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

int bloomfilter_add(char * message,int *bloom){
unsigned long a;
int i;
for(i=1;i<=8;i++)
{
	a=hash(message,i);
	SetBit(bloom,a);
	
}return 0;
}

int bloomfilter_check(char * message,int *bloom){
	unsigned long a;
	int i;
	for(i=1;i<=8;i++)
	{
		a=hash(message,i);
		if(TestBit(bloom,a)==0){
	//		printf(RED"The string is not storred\n"RESET);
			return 0;
		}
	//returns 0 if there is one bit 0, or 1 if all of them are 1.	
	}
//printf(YELLOW"The string may be storred\n"RESET);
return 1;
}

