
CC = gcc
CFLAGS  = -g -Wall

default: project

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#

project:  main.o functions.o stack.o bloomfilter.o murmur3.o top.o
	$(CC) $(CFLAGS) -o project main.o functions.o stack.o bloomfilter.o murmur3.o top.o

test_project:  test_main.o functions.o stack.o test.o bloomfilter.o murmur3.o top.o
	$(CC) $(CFLAGS) -o test_project test_main.o functions.o stack.o test.o bloomfilter.o murmur3.o top.o

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

bloomfilter.o:  bloomfilter.c bloomfilter.h 
	$(CC) $(CFLAGS) -c bloomfilter.c

stack.o:  stack.c stack.h 
	$(CC) $(CFLAGS) -c stack.c

murmur3.o:  murmur3.c murmur3.h
	$(CC) $(CFLAGS) -c murmur3.c

top.o:  top.c top.h
	$(CC) $(CFLAGS) -c top.c
	


clean: 
	$(RM) count *.o *~ diffs

testrun:
	  valgrind --leak-check=yes ./project -q small.work -i small.init 

testrunv:
	  valgrind --leak-check=yes -v ./project -q small.work -i small.init 

testrun2:
	  valgrind --leak-check=yes  ./project -q test.work -i test.init 

testrun3:
	  valgrind --leak-check=full --show-leak-kinds=all ./project -q test.work -i test.init 

run:
	time ./project -q small.work -i small.init 
pipe:
	time ./project -q small.work -i small.init > results.txt


