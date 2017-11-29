
CC = gcc
CFLAGS  = -g -Wall

default: project

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#

project:  main.o functions.o stack.o static_functions.o
	$(CC) $(CFLAGS) -o project main.o functions.o stack.o static_functions.o -lm

test_project:  test_main.o functions.o stack.o test.o
	$(CC) $(CFLAGS) -o test_project test_main.o functions.o stack.o test.o -lm

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
main.o:  main.c functions.h
	$(CC) $(CFLAGS) -c main.c

test_main.o:  test_main.c functions.h
	$(CC) $(CFLAGS) -c test_main.c



test.o: test.c functions.h
	$(CC) $(CFLAGS) -c test.c

static_functions.o:  static_functions.c static_functions.h 
	$(CC) $(CFLAGS) -c static_functions.c

functions.o:  functions.c functions.h 
	$(CC) $(CFLAGS) -c functions.c

stack.o:  stack.c stack.h 
	$(CC) $(CFLAGS) -c stack.c

clean: 
	$(RM) count *.o *~ diffs

testrun:
	  valgrind --leak-check=yes ./project -q small.work -i small.init 

testrun2:
	  valgrind --leak-check=yes ./project -q test.work -i test.init

testrun3:
	  valgrind --tool=callgrind ./project -q medium_dynamic.work -i medium_dynamic.init 

run:
	./project -q small.work -i small.init 
run2:
	./project -q test.work -i test.init 

run3:
	./project -q medium_dynamic.work -i medium_dynamic.init 

pipe:
	  ./project -q small.work -i small.init > results.txt


