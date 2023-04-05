CC = gcc
CFLAGS =  -Wall -g -std=c90
LDFLAGS =  -lm

all: main.o libfila.o libconjunto.o liblef.o entidades.o eventos.o
	$(CC) $(CFLAGS)  main.o libfila.o libconjunto.o liblef.o entidades.o eventos.o -o simulador $(LDFLAGS)

main.o : main.c	
	$(CC) $(CFLAGS) -c main.c

libfila.o : libfila.c
	$(CC) $(CFLAGS) -c libfila.c

libconjunto.o : libconjunto.c
	$(CC) $(CFLAGS) -c libconjunto.c

liblef.o : liblef.c
	$(CC) $(CFLAGS) -c liblef.c

entidades.o : entidades.c
	$(CC) $(CFLAGS) -c entidades.c 

eventos.o : eventos.c
	$(CC) $(CFLAGS) -c eventos.c

clean :
	rm -f *.o simulador
