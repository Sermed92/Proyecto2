#ifndef libreria
#define libreria

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

// Archivo de salida
        FILE *salida;

// Contadores de archivos, directorios e hilos
	int numero_archivos;
	int numero_directorios;
	int numero_hilos;

// Funciones y procedimientos a utilizar
void numeroArgumentos(int);
int es_directorio(char*);
void encolar(char*);
char *desencolar();
void procesar_directorio(char*);
void agregar_slash(char*);
void* trabajo_de_hilo(void*);
char *mi_strdup(char*);

#endif // !libreria