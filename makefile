
CC = gcc
CFLAGS  = -g -Wall

default: project

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#

project:  main.o functions.o stack.o
	$(CC) $(CFLAGS) -o project main.o functions.o stack.o

test_project:  test_main.o functions.o stack.o test.o
	$(CC) $(CFLAGS) -o test_project test_main.o functions.o stack.o test.o

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
main.o:  main.c functions.h
	$(CC) $(CFLAGS) -c main.c

test_main.o:  test_main.c functions.h
	$(CC) $(CFLAGS) -c test_main.c

test.o: test.c functions.h
	$(CC) $(CFLAGS) -c test.c

functions.o:  functions.c functions.h 
	$(CC) $(CFLAGS) -c functions.c

stack.o:  stack.c stack.h 
	$(CC) $(CFLAGS) -c stack.c

clean: 
	$(RM) count *.o *~ diffs

testrun:
	  valgrind --leak-check=yes ./project -q small.work -i small.init 


run:
	./project -q small.work -i small.init 
pipe:
	  ./project -q small.work -i small.init > results.txt

testpipe:
	  valgrind --leak-check=yes ./project -q small.work -i small.init > results.txt

small:
	valgrind --leak-check=yes ./project -q small.work -i small.init 

diffs:
	gcc diffs.c -o diffs 
	./diffs 
