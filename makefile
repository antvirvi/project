
CC = gcc
CFLAGS  = -g -Wall

default: project

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
<<<<<<< HEAD
project:  main.o functions.o stack.o
	$(CC) $(CFLAGS) -o project main.o functions.o stack.o
=======
project:  main.o functions.o 
	$(CC) $(CFLAGS) -o project main.o functions.o
>>>>>>> ec2bf7ae61c93922e7b02d4728a9cb475f034d69

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
main.o:  main.c functions.h 
	$(CC) $(CFLAGS) -c main.c


functions.o:  functions.c functions.h 
	$(CC) $(CFLAGS) -c functions.c

stack.o:  stack.c stack.h 
	$(CC) $(CFLAGS) -c stack.c

clean: 
	$(RM) count *.o *~
