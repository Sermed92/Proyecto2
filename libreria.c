#include "libreria.h"

const char *MENSAJE_ERROR_AYUDA = "Error! - La opcion -h excluye las demas opciones";
const char *MENSAJE_AYUDA = "Uso de tls:\n./tls [-h] | [-n <i>] [-d <directorio>] [salida] \n-h: Muestra por pantalla un mensaje de ayuda, excluye del resto de opciones\n-n i: Nivel de concurrencia solicitado (uno por defecto)\n-d directorio: directorio desde donde calcula el espacio utilizado (actual por defecto)\nsalida: Archivo con lista de directorios y el espacio ocupado por los archivos regulares (salida estandar por defecto)";
const char *MENSAJE_ERROR_ABRIR_ARCHIVO = "Error! - No se pudo abrir archivo";

// Finaliza el programa si no posee la cantidad de argumentos necesarios, despues de indicar la forma correcta
void numeroArgumentos(int cantidad){
	if ((cantidad < 1) || (cantidad > 6)){
		printf("Violacion de parametros \n");
		printf("./UsoDisco [-h] | [-n <i>] [-d <directorio>] [salida] \n");
		exit(1);
	}
}

// Funcion para manejar la opcion -h si ha sido ingresada
void opcion_help(int h, int cantidad){
	// Se finaliza el programa si se introdujo una opcion adicional a -h
	if (h == 1 && cantidad > 2) {
		printf("%s\n", MENSAJE_ERROR_AYUDA);
		exit(1);
 	}
	// Se imprime por salida estandar el mensaje de ayuda
	if (h == 1) {
		printf("%s\n",MENSAJE_AYUDA);
		exit(0);
	}
}

// Funcion para verificar si un arhivo es oculto
int es_oculto(char *direccion) {
	char *comprobante = NULL;
	comprobante = strstr(direccion, ".");
	// Si la direccion empieza con un punto se asume que la direccion representa un enlace oculto
	if (comprobante != direccion) {
		return 0;
	}
	return 1;
}

//Funcion para determinar si un string representa un directorio
int es_directorio(char* direccion){
	// Se utiliza stat para determinar si una direccion representa un directorio
	struct stat buffer;
	if (stat(direccion, &buffer)) {
		printf("No se pudo aplicar stat sobre %s\n", direccion);
		// exit(1);
	}
	int x = S_ISDIR(buffer.st_mode);
	return x;
}

// Procedimiento para encolar un directorio en la cola global de directorios
void encolar(char *directorio) {
	// Se reserva memoria para un nuevo nodo
	nodo *nuevo_nodo = (nodo*) malloc(sizeof(nodo));
	nuevo_nodo -> directorio = strdup(directorio);
	nuevo_nodo -> siguiente = NULL;

	// Se colocan tanto cabeza como cola en el nuevo nodo si ambos son nulos
	if (cabeza == NULL && cola == NULL) {
		cabeza = nuevo_nodo;
		cola = cabeza;
	}
	// Se coloca el nuevo nodo al inicio de la cola si ya existen otros nodos
	else {
		cola -> siguiente = nuevo_nodo;
		cola = nuevo_nodo;
	}
}

// Funcion para desencolar un directorio de la cola global de directorios
char *desencolar() {
	// Se retorna NULL si la cola esta vacia
	if (cabeza == NULL) {
		return NULL;
	}
	char *directorio = strdup(cabeza -> directorio);
	nodo *Head = cabeza;
	cabeza = cabeza -> siguiente;
	if (cabeza == NULL) {
		cola = NULL;
	}
	// Se libera la memoria del nodo y se retorna una copia de la informacion que contenia
	free(Head);
	return directorio;
}

// Funcion que agrega un slash al final de un string si este no lo posee
void agregar_slash(char *direccion) {
	char ultimo = direccion[strlen(direccion)-1];
	if (strcmp(&ultimo,"/") != 0) {
		strcat(direccion,"/");
	}
}

// Funcion auxiliar para concatenar dos strings
char *mi_strcat(char *s1, char *s2) {
	int n1 = strlen(s1);
	int n2 = strlen(s2);
	char *nuevo = (char*) malloc(sizeof(char) * (n1 + n2 + 1));
	int i, j;
	for (i = 0; i < n1; i++) {
		nuevo[i] = s1[i];
	}
	for (j = 0; j < n2; j++, i++) {
		nuevo[i] = s2[j];
	}
	nuevo[i] = '\0';
	return nuevo;
}

// Funcion para procesar un archivo retornando el numero de bytes que ocupa
int procesar_archivo(char *direccion) {
	// Se utiliza la funcion stat para obtener el numero de bytes de un archivo
	struct stat buffer;
	if (stat(direccion, &buffer)) {
		printf("No se pudo aplicar stat sobre %s\n", direccion);
		exit(1);
	}
	return (int) buffer.st_size;
}

// Funcion para procesar un directorio
void procesar_directorio(char *direccion) {
	agregar_slash(direccion);
	// Se utiliza la funcion dirent para recorrer el directorio recibido
	struct dirent **lista;
	// Se utiliza la variable aux para el manejo de strings
	char *aux;
	aux = (char*) strdup(direccion);
	// En la variable n se almacena la cantidad de enlaces en la direccion
	int n = scandir(direccion, &lista, NULL, alphasort);
	// Se suman el total de bytes en la direccion y la cantidad de archivos locales
	int total_bytes = 0;
	int archivos_locales = 0;
	// Se obtiene el identificador del hilo actual para la salida
	pthread_t self;
	self = pthread_self();

	if( n< 0) {
		printf("Error al abrir directorio %s\n", direccion);
		exit(1);
	} else {
		int i;
		for (i = 0; i < n; i++)
		{

			if (!es_oculto(lista[i] -> d_name)) {
				// Si el enlace actual no es oculto se concatena con la direccion recibida para tener la direccion completa
				aux = mi_strcat(aux,lista[i] -> d_name);

				if (!es_directorio(aux)) {
					// Si no es un directorio se toma su cantidad de bytes y se incrementa el contador de archivos regulares
					total_bytes += procesar_archivo(aux);
					archivos_locales++;
				} else {
					// Si es un directorio se encola en la cola de directorios por explorar
					encolar(aux);
				}
			}

			free(lista[i]);
			free (aux);
			// Se reinicia la variable aux para la siguiente iteracion
			aux = (char*) strdup(direccion);
		}
	}
	free(lista);
	// Se imprime la salida de manera estandar o en el archivo de salida segun sea el caso
	if (salida == NULL) {
		printf("Hilo: %d\nDirectorio: %s\nBytes: %d\nCantidad de archivos: %d\n\n\n", (int) self, direccion, total_bytes, archivos_locales);
	} else {
		fprintf(salida, "Hilo %d\nDirectorio: %s\nBytes: %d\nCantidad de archivos: %d\n\n\n",(int) self, direccion, total_bytes, archivos_locales);
	}
}

// Funcion sobre la cual trabajaran los hilos creados
void* trabajo_de_hilo(void *arg) {
	// Se castea el argumento recibido para que sea compatible con la funcion procesar_directorio
	char *direccion = (char*) arg;
	procesar_directorio(direccion);
	return NULL;
}

// Funcion para crear el archivo de salida de ser necesario
FILE *crear_salida(int posicion, int cantidad, char** argumentos) {
	// Si no se procesaron todos los argumentos, existe un archivo de salida
	if (posicion < cantidad) {
		if ((salida = fopen(argumentos[posicion], "w")) == NULL) {
			printf("%s %s\n",MENSAJE_ERROR_ABRIR_ARCHIVO, argumentos[posicion]);
			exit(1);
		}
		else {
			return salida;
		}
	}

	return salida;
}

// Procedimiento para cerrar el archivo de salida de ser necesario
void cerrar_salida(int optind, int argc) {
	if (optind < argc) {
        // Se cierra el archivo una vez escrito
		if (fclose(salida) != 0) {
			printf("No se pudo cerrar el archivo de salida\n");
			exit(1);
		}
	}
}

// Procedimiento para crear los hilos trabajadores
void crear_hilos(int n_hilos) {
	if (n_hilos == 0) {
		// Si no se recibe ningun valor por el argumento n, se trabaja solo en el programa principal
		while (cabeza != NULL) {
			if (cabeza != NULL) {
				procesar_directorio(desencolar());
			}
		}
	} else {
        // Se declara el arreglo de hilos
		pthread_t arreglo_hilos[n_hilos];
		int cont = 0;
		int i;
		char * path;

		while (cabeza != NULL) {
			path = desencolar();
            //Se crean los hilos y se les asigna un directorio (path) con el cual trabajar
			pthread_create(&arreglo_hilos[cont], NULL, trabajo_de_hilo, path);
            cont++;

			if (cont == n_hilos) {
				cont = 0;
				for (i = 0; i < n_hilos; i++) {
					pthread_join(arreglo_hilos[i], NULL);
				}
			}
		}
		// Se espera a que todos los hilos terminen
		for (i = 0; i < n_hilos; i++) {
			pthread_join(arreglo_hilos[i], NULL);
		}
	}
}

// Procedimiento para obtener el directorio actual de trabajo o obtener el directorio pasado por parametros
void asignar_directorio(char **directorio) {
	if (*directorio == NULL){
		// Si directorio es NULL se trabaja en el directorio actual
		char directorio_actual[BUFSIZE];
		char *cp;
		// Se obtiene el directorio actual de trabajo
		cp = getcwd(directorio_actual, sizeof(directorio_actual));
		if (cp == NULL) {
			printf("Error obteniendo el directorio actual\n");
			exit(1);
		}
		*directorio = directorio_actual;
	}

	// directorio no es NULL == Se trabaja con el directorio pasado por opciones
	if (!es_directorio(*directorio))
	{
		printf("El argumento '%s' no es un directorio\n", *directorio);
		exit(1);
	}
}
