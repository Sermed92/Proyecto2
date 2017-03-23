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
#include <libgen.h>

#define BUFSIZE 4096

// Estructura para cola de directorios
	typedef struct _nodo {
		char *directorio;
		struct _nodo *siguiente;
	} nodo;

/* Variables globales */
// Apuntadores usados para la cola global de directorios
	nodo *cabeza;
	nodo *cola;

// Archivo de salida
	FILE *salida;

// Funciones y procedimientos a utilizar
	void numeroArgumentos(int);
	int es_directorio(char*);
	void encolar(char*);
	char *desencolar();
	void procesar_directorio(char*);
	void agregar_slash(char*);
	void* trabajo_de_hilo(void*);
	char *mi_strdup(char*);
	void opcion_help(int, int);
	FILE *crear_salida(int, int, char**);
	void cerrar_salida(int, int);
	void crear_hilos(int);
	void asignar_directorio(char **directorio);

#endif // !libreria
