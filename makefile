all: UsoDisco clean

UsoDisco: main.o
	gcc -std=c99 -pthread -o UsoDisco main.o

main.o: main.c
	gcc -Wall -Wextra -c main.c

debug: main.c batalla.c
	gcc -g -c main.c
	gcc -g -c batalla.c
	gcc -o batalla main.o

clean:
	rm -f *.o