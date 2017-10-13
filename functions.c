//#include "functions.h"
#include "libraries.h"

extern int buffer_size;




void printpanos(void)
{
	printf("Hello Panos\n");
}

int openinput(char * filename)
{	
	int flag; //1 question, 2 addition, 3 deletion, 4 end of file
	char* buffer =  malloc(buffer_size*sizeof(char));  //creating a char table to store orders from input file
	char word[1024];
	buffer[1] = 'a';
	FILE* fd = fopen(filename, "r"); //opening input file
	if(fd == NULL)
	{
		perror("Error opening input file");
		return -1;
	}



	while (flag!=4)
	{
		fscanf(fd, " %1023s", word);		

		if(strcmp(word,"Q")==0)
		{
			printf("\nQuestion:");
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

		else if(strcmp(word,"F")==0)
		{
			printf("End of file\n");
			flag = 4;
		}
		else
		{
			switch(flag)
			{
		
				case 1 :
					//search
		printf("%s ",word);
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

		//printf("Word: %s ",word);
    }

 
/*	while(read = fgets(buffer, buffer_size, fd))
	{ 
		if(strcmp(buffer,"F\n")==0)
			printf("End of input file\n");
		else
			printf("%s",buffer);
		
	}
*/

	return 0;
}
