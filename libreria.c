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

void opcion_help(int h, int cantidad){
	if (h == 1 && cantidad > 2) {
		printf("%s\n", MENSAJE_ERROR_AYUDA);
		exit(1);
 	}
	if (h == 1) {
		printf("%s\n",MENSAJE_AYUDA);
		exit(0);
	}
}

// Funcion para verificar si un arhivo es oculto
int es_oculto(char *direccion) {
	char *comprobante = NULL;
	comprobante = strstr(direccion, ".");
	if (comprobante != direccion) {
		return 0;
	}
	return 1;
}

//Funcion para determinar si un string representa un directorio
int es_directorio(char* direccion){

	struct stat buffer;
	if (stat(direccion, &buffer)) {
		printf("No se pudo aplicar stat sobre %s\n", direccion);
	}
	int x = S_ISDIR(buffer.st_mode);
	return x;
}

// Procedimiento para encolar un directorio en la cola global de directorios
void encolar(char *directorio) {
	nodo *nuevo_nodo = (nodo*) malloc(sizeof(nodo));
	nuevo_nodo -> directorio = strdup(directorio);
	nuevo_nodo -> siguiente = NULL;

	if (cabeza == NULL && cola == NULL) {
		cabeza = nuevo_nodo;
		cola = cabeza;
	}
	else {
		cola -> siguiente = nuevo_nodo;
		cola = nuevo_nodo;
	}
}

// Funcion para desencolar un directorio de la cola global de directorios
char *desencolar() {
	if (cabeza == NULL) {
		return NULL;
	}
	char *directorio = strdup(cabeza -> directorio);
	nodo *Head = cabeza;
	cabeza = cabeza -> siguiente;
	if (cabeza == NULL) {
		cola = NULL;
	}
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

// Funcion para procesar un archivo
int procesar_archivo(char *direccion) {

	struct stat buffer;
	if (stat(direccion, &buffer)) {
		printf("No se pudo aplicar stat sobre %s\n", direccion);
	}

	return (int) buffer.st_blocks;
}

// Funcion para procesar un directorio
void procesar_directorio(char *direccion) {
	agregar_slash(direccion);
	struct dirent **lista;
	char *aux;
	aux = (char*) strdup(direccion);
	int n = scandir(direccion, &lista, NULL, alphasort);
	int total_bytes = 0;
	int archivos_locales = 0;
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
				aux = mi_strcat(aux,lista[i] -> d_name);

				if (!es_directorio(aux)) {
					total_bytes += procesar_archivo(aux);
					archivos_locales++;
				} else {
					encolar(aux);
				}
			}

			free(lista[i]);
			free (aux);
			aux = (char*) strdup(direccion);
		}
	}
	free(lista);
	if (salida == NULL) {
		printf("Hilo: %d\nDirectorio: %s\nBytes: %d\nCantidad de archivos: %d\n\n\n", (int) self, direccion, total_bytes, archivos_locales);
	} else {
		fprintf(salida, "Hilo %d\nDirectorio: %s\nBytes: %d\nCantidad de archivos: %d\n\n\n",(int) self, direccion, total_bytes, archivos_locales);
	}
}

void procesar_directorio1(char *direccion) {
	agregar_slash(direccion);
	DIR *dir;
	struct dirent *dp;
	char* aux;
	aux = (char*) strdup(direccion);

	if((dir = opendir(direccion)) == NULL) {
		printf("Error! No se pudo abrir directorio: %s\n", direccion);
		exit(1);
	}

	while ((dp = readdir(dir)) != NULL) {
		if(!es_oculto(dp->d_name)) {
			// printf("%s\n", dp->d_name);
			aux = mi_strcat(aux, dp->d_name);
			// printf("aux: %s\n", aux);
			if(es_directorio(aux)) {
				encolar(aux);
			}
		}
		free(aux);
		aux = (char*) strdup(direccion);
	}
}

// Funcion sobre la cual trabajaran los hilos creados
void* trabajo_de_hilo(void *arg) {
	char *direccion = (char*) arg;
	procesar_directorio(direccion);
	return NULL;
}

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

void cerrar_salida(int optind, int argc) {
	if (optind < argc) {
        // Se cierra el archivo una vez escrito
		if (fclose(salida) != 0) {
			printf("No se pudo cerrar el archivo de salida\n");
			exit(1);
		}
	}
}

void crear_hilos(int n_hilos) {
	if (n_hilos == 0) {
		// Si no se recibe ningun valor por el argumento n, se trabaja solo en el programa principal
		while (cabeza != NULL) {
			procesar_directorio(desencolar());
		}
	} else {
        // Se declaraa el arreglo de hilos
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
		for (i = 0; i < n_hilos; i++) {
			pthread_join(arreglo_hilos[i], NULL);
		}
	}
}

void asignar_directorio(char **directorio) {
	if (*directorio == NULL){
		// Si directorio es NULL se trabaja en el directorio actual
		char directorio_actual[BUFSIZE];
		char *cp;
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
