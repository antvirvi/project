
CC = gcc
CFLAGS  = -g -Wall

default: project

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
project:  main.o functions.o 
	$(CC) $(CFLAGS) -o project main.o functions.o

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
main.o:  main.c functions.h 
	$(CC) $(CFLAGS) -c main.c


functions.o:  functions.c functions.h 
	$(CC) $(CFLAGS) -c functions.c

clean: 
	$(RM) count *.o *~
