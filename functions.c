//#include "functions.h"
#include "libraries.h"

extern int buffer_size;
extern int word_size;
extern int table_size;
int reset = 0;



char *getWord(FILE *fp)			//returns the contents of fp word by word, ignoring non alpharithmetic symbols
{
	char ch;
	char *buf = malloc(word_size*sizeof(char));
	int i = 0, flag = 0;

	while(i<word_size)
	{
		ch = getc(fp);
		if(ch == '\n')
			reset = 1;				//flag stating that this is the last word of a line
		if(!isalnum(ch))
		{
			if(flag == 1)
				return buf;			//if it has read letters and then receives a space
		}
		else
		{		
			if(i==word_size-1) //criterion for double size buffer
			{
				char * help_buffer = malloc(word_size*sizeof(char));
				word_size*=2;		//double the size
				strcpy(help_buffer,buf);
				buf=realloc(buf, word_size*sizeof(char));
				strcpy(buf,help_buffer);
				printf("Double the size of a word\n");
			}
			flag = 1;
			buf[i] = ch;
			i++;
		}
	
	}
return "error";
}

int openinput(char * filename)
{
	char **ptr_table = malloc(table_size*sizeof(char *));
	int words_in = 0;
	int flag; //1 question, 2 addition, 3 deletion, 4 end of file
	char* buffer =  malloc(buffer_size*sizeof(char));  //creating a char table to store orders from input file
	char word[100];

	FILE* fd = fopen(filename, "r"); //opening input file
	strcpy(buffer,"\0");

	if(fd == NULL)
	{
		perror("Error opening input file");
		return -1;
	}


	while (flag!=4)
	{
		printf("Hello\n");
		if(reset==1)
		{
			printf("reset bitzes!\n");
			//buffer = realloc(buffer,buffer_size*sizeof(char));
			//printf("Reseted buffer: %s\n",buffer);
			//call some function to add, delete or search
			reset = 0;
		}
		strcpy(word,getWord(fd));

		if(strcmp(word,"Q")==0)
		{
			printf("\nQuestion:\n");
			flag = 1;
		}

		else if(strcmp(word,"A")==0)
		{
			printf("\nAddition:");
			flag = 2;
		}

		else if(strcmp(word,"D")==0)
		{
			printf("\nDeletion");
			flag = 3;
		}
		else if(strcmp(word,"\n")==0)
		{
			printf("\nReset");
			flag = 3;
		}

		else if(strcmp(word,"F")==0)
		{
			printf("\nEnd of file\n");
			flag = 4;
		}
		else
		{

			if(strlen(buffer)+strlen(word)>buffer_size)  // if we are about to overflow, we need to double the size
			{
				printf("world\n");
				char * help_buf = malloc(buffer_size*sizeof(char));
				strcpy(help_buf,buffer);

				buffer_size *= 2;
				buffer= realloc(buffer,buffer_size*sizeof(char));
				buffer = strcat(help_buf,word);
//				printf("Prevented overflow by doubling buffer size to %d\n",buffer_size);
			}
			else
			{

				if(words_in==table_size - 1)
				{
					printf("world2\n");
					table_size*=2;
					ptr_table = realloc(ptr_table,table_size*sizeof(char*));
				}
				printf("%s \n",word);
				ptr_table[words_in] = malloc(word_size*sizeof(char));
				strcpy(ptr_table[words_in],word);

				words_in++;
			}


			switch(flag)
			{
		
				case 1 :
					//search
						printf("%s ",word);
						
//						insert_ngram
						break;
				case 2 :
					//add
						printf("%s ",word);
						break;
				case 3 : 
					//delete
						printf("%s ",word);
						break;
			}
		}

    }

	return 0;
}
