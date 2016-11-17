#ifndef cabecera
#define cabecera

/* Librerias */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <dirent.h>

// Estructura para cola de directorios
typedef struct _nodo_cola {
	char *directorio;
	struct _nodo_cola *siguiente;
} nodo_cola;

/* Variables globales */
// Apuntadores usados para la cola global de directorios
	nodo_cola *cabeza;
	nodo_cola *cola;

// Archivo se salida
        FILE *salida;

void numeroArgumentos(int);
int es_directorio(char*);

void encolar(char*);
char *desencolar();

void procesar_directorio(char*);
void agregar_slash(char*);

#endif // !cabecera