all: UsoDisco clean

UsoDisco: main.o libreria.o libreria.h
	gcc -std=c99 -pthread -g -o UsoDisco main.o libreria.o

main.o: main.c libreria.h
	gcc -Wall -Wextra -g -c main.c

libreria.o: libreria.c libreria.h
	gcc -Wall -Wextra -g -c libreria.c

clean:
	rm -f *.o