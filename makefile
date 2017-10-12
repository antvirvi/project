#all: project
#
#project: main.o functionc.o
#	gcc -o project main.o functions.o
#
#main.o: main.c
#	gcc main.o -o main.c
#
#functions.o: functions.c
#	gcc functions.o -o functions.c
#
#functions: functions.o
#	gcc functions.c -c functions
#
#clean:
#	rm *.o project
all:
	gcc main.o -c main.c
	gcc functions.o -c functions.c
