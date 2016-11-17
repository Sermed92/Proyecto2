#include "cabecera.h"

// Finaliza el programa si no posee la cantidad de argumentos necesarios, despues de indicar la forma correcta
void numeroArgumentos(int cantidad){
	if ((cantidad < 1) || (cantidad > 6)){
		printf("Violacion de parametros \n");
		printf("./UsoDisco [-h] | [-n <i>] [-d <directorio>] [salida] \n");
		exit(1);
	}
}

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

	// struct stat statbuf;

	// if (lstat(dir, &statbuf) == 1){
	// 	fprintf(stderr, "No se pudo aplicar stat sobre el archivo %s: %s\n", dir, strerror(errno));
	// 	exit(1);
	// }

	// // Se verifica si es directorio
	// if (statbuf.st_mode & S_IFDIR){
	// 	// Es directorio
	// 	return 1;
	// } else {
	// 	// No es directorio
	// 	return 0;
	// }

	// return 0;
	//printf("Recibo %s\n", direccion);

	DIR *directorio;
	if ((directorio = opendir(direccion)) != NULL) {
		closedir(directorio);
		return 1;
	}
	else {
		return 0;
	}

}

// Procedimiento para encolar un directorio en la cola global de directorios
void encolar(char *directorio) {
	printf("Encole %s\n", directorio);
	nodo_cola *nuevo_nodo = (nodo_cola*) malloc(sizeof(nodo_cola));
	nuevo_nodo -> directorio = directorio;
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
	if (cola == NULL) {
		return NULL;
	}
	char *directorio = cabeza -> directorio;
	nodo_cola *nodo = cabeza;
	cabeza = cabeza -> siguiente;
	if (cabeza == NULL) {
		cola = NULL;
	}
	free(nodo);
	return directorio;
}

// Funcion que agrega un slash al final de un string si este no lo posee
void agregar_slash(char *direccion) {
	char ultimo = direccion[strlen(direccion)-1];
	if (strcmp(&ultimo,"/") != 0) {
		strcat(direccion,"/");
	}
}

void procesar_directorio(char *direccion) {
	// char *string_aux = strdup(raizN);
	// printf("Auxiliar %s", string_aux);
	// DIR *directorio;
	// struct dirent *actual;

	// if ((directorio = opendir(direccion)) != NULL) {
	// 	while ((actual = readdir(directorio)) != NULL) {
	// 		if (!es_oculto(actual -> d_name)) {

	// 			// char slash[2] = "/\0";

	// 			// strcat(&slash, actual -> d_name);
	// 			strcat(direccion,actual -> d_name);

	// 			printf("No es oculto %s\n", direccion);

	// 			if (es_directorio(actual -> d_name)) {
	// 				// Si lo encontrado es un directorio se encola
	// 				printf("Encolo %s\n", actual -> d_name);
	// 				encolar(actual -> d_name);
	// 			}
	// 			else {
	// 				printf("%s%s\n",direccion,actual -> d_name);
	// 				// Procesar archivo
	// 			}
	// 		}
	// 	}
	// 	closedir(directorio);
	// }
	// else {
	// 	printf("No se pudo abrir directorio %s\n", direccion);
	// 	exit(1);
	// }
	//char *d = agregar_slash(direccion);
	char *d = direccion;
	agregar_slash(d);
	printf("Direccion: %s\n", d);
	DIR *directorio;
	struct dirent *actual;

	char *auxiliar = strdup(d);
	if((directorio = opendir(auxiliar)) != NULL) {
		while ((actual = readdir(directorio)) != NULL) {
			if (!es_oculto(actual -> d_name)) {
				strcat(auxiliar, actual -> d_name);
				printf("Proceso: %s\n",auxiliar);
				if (!es_directorio(auxiliar)) {
					//Se procesa el archivo cuya direccion esta en auxiliar
					printf("A: %s\n", auxiliar);
				} else {
					// Si es directorio se encola
					encolar(auxiliar);
				}
				auxiliar = strdup(d);
			}
		}
	}
}