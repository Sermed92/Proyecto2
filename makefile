all: UsoDisco clean

UsoDisco: main.o libreria.o cabecera.h
	gcc -std=c99 -pthread -g -o UsoDisco main.o libreria.o

main.o: main.c cabecera.h
	gcc -Wall -Wextra -g -c main.c

libreria.o: libreria.c cabecera.h
	gcc -Wall -Wextra -g -c libreria.c

clean:
	rm -f *.o