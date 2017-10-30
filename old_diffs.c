#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char ** argv) {

	FILE *fp1, *fp2;
	int ch1, ch2,line = 0,flag=0;
	char fname1[40], fname2[40];
	if(argc<3){
		printf("Enter name of first file :");
		gets(fname1);

		printf("Enter name of second file:");
		gets(fname2);
	}
	else{
		strcmp(fname1,argv[1]);
		strcmp(fname2,argv[2]);
	}
	printf("%s %s\n",fname1,fname2);
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");

	if (fp1 == NULL) {
		printf("Cannot open %s for reading ", fname1);
		exit(1);
	} else if (fp2 == NULL) {
		printf("Cannot open %s for reading ", fname2);
		exit(1);
	} else {
		ch1 = getc(fp1);
		ch2 = getc(fp2);
		printf("This function checks in which line is the first character difference in two files\n");
		while ((ch1 != EOF) && (ch2 != EOF)) {
			ch1 = getc(fp1);
			ch2 = getc(fp2);

			if (ch2=='\n')
				line++;
			if (ch1 != ch2){
				printf("Difference in line #%d of second file\n",line);
				printf("Files are Not identical \n");
				flag++;
				fclose(fp1);
				fclose(fp2);
				return -1;
			}
		}

		if (flag==0)
			printf("Files are identical n");
		else if (flag){
			printf("Files are Not identical n");
			int a;
			printf("Total diffs %d characters\n",flag);
		}
	fclose(fp1);
	fclose(fp2);
	}
	return (0);
}
