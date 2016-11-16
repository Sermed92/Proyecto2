all: UsoDisco clean

UsoDisco: main.o libreria.o cabecera.h
	gcc -std=c99 -pthread -o UsoDisco main.o libreria.o

main.o: main.c cabecera.h
	gcc -Wall -Wextra -c main.c

libreria.o: libreria.c cabecera.h
	gcc -Wall -Wextra -c libreria.c

debug: main.c libreria.c
	gcc -g -c main.c
	gcc -g -c libreria.c
	gcc -o libreria main.o

clean:
	rm -f *.o