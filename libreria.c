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
	
	struct stat buffer;
	if (stat(direccion, &buffer)) {
		printf("No se pudo aplicar stat sobre %s\n", direccion);
	}
	int x = S_ISDIR(buffer.st_mode);
	return x;
}

// Procedimiento para encolar un directorio en la cola global de directorios
void encolar(char *directorio) {
	nodo_cola *nuevo_nodo = (nodo_cola*) malloc(sizeof(nodo_cola));
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

void procesar_archivo(char *direccion) {
	numero_archivos++;
	struct stat buffer;
	if (stat(direccion, &buffer)) {
		printf("No se pudo aplicar stat sobre %s\n", direccion);
	}	

	if (salida == NULL){
		printf("%i\t%s\n",(int) buffer.st_blocks ,direccion);
	} else {
		fprintf(salida, "%i\t%s\n",(int) buffer.st_blocks ,direccion);
	}
	
}

// Funcion para procesar un directorio
void procesar_directorio(char *direccion) {
	numero_directorios++;
	agregar_slash(direccion);
	struct dirent **lista;
	char *aux;
	aux = (char*) strdup(direccion);
	int n = scandir(direccion, &lista, NULL, alphasort);
	
	if( n< 0) {
		printf("Error al abrir directorio %s\n", direccion);
		exit(1);
	} else {
		for (int i = 0; i < n; i++)
		{
	
			if (!es_oculto(lista[i] -> d_name)) {
				aux = mi_strcat(aux,lista[i] -> d_name);
	
				if (!es_directorio(aux)) {
					procesar_archivo(aux);
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

}