
# lab11a.o is also compiled using -ggdb option

ex11q1: ex11q1.c lab11.h
	gcc -Wall -std=c99 -ggdb ex11q1.c lab11.o -o ex11q1

all: lab11.o ex11q1

lab11.o: lab11.c lab11.h
	gcc -Wall -std=c99 -ggdb -c lab11.c

tar:
	tar -cvf submit.tar lab11.h lab11.o ex11q1.c makefile
