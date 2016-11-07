/*
 * Archivo: main.c
 * Autores:
 *          Sergio Medina 09-11259
 *          Lucio Mederos 13-10856
 * Descripción:   
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcnt1.h>

#include <errno.h>
#include <ctype.h>
#include <pthread.h>


// Finaliza el programa si no posee la cantidad de argumentos necesarios, despues de indicar la forma correcta
 void numeroArgumentos(int cantidad){
    if ((cantidad < 1) || (cantidad > 4)){
        printf("Violacion de parametros \n");
        printf("./UsoDisco [-h] | [-n <i>] [-d <directorio>] [salida] \n");
        exit(1);
    }
 }

 int es_dir(char* dir){

 	struct stat statbuf;

 	if (lstat(dir, &statbuf) == 1){
 		fprintf(stderr, "No se pudo aplicar stat sobre el archivo %s: %s\n", dir, strerror(errno));
 		exit(1);
 	}

 	// Se verifica si es directorio
 	if (statbuf.st_mode & S_IFDIR){
 		// Es directorio
 		return 1;
 	} else {
 		// No es directorio
 		return 0;
 	}
 }

 int main(int argc, char** argv){

 	numeroArgumentos(argc);

 	int hflag = 0;
 	int nvalue = 0;
 	char *dvalue = NULL;
 	int n;

 	opterr = 0;

 	while ((n = getopt(argc,argv, "hnd:")) != -1){
 		switch(n){
 			case'h':
 			{
 				hflag = 1;
 				break;
 			}
 			case 'd':
 			{
 				dvalue = optarg;
 				break;
 			}
 			case 'n':
 			{
 				nvalue = atoi(optarg);
 				break;
 			}
 			case '?':
 			{
 				if (optopt == 'n'){
 					fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
 				} else if (isprint(optopt)){
 					fprintf(stderr, "Opcion desconocida '-%c'.\n", optopt);
 				} else {
 					fprintf(stderr, "Caracter desconocido '\\x%x'.\n", optopt);
 				}
 				return 1;
 			}
 			default:
 			{
 				abort();
 			}
 		}
 	}

 	if ((hflag == 1) && (argc > 2)) {
 		fprintf(stderr, "La opcion '-h' es excluyente del resto de las opciones%s\n", );
 		exit(1);
 	}

 	if (hflag == 1){
 		printf("UsoDisco Usage:\n");
        printf("./UsoDisco [-h] | [-n <i>] [-d <directorio>] [salida] \n");
        printf("-h: Muestra por pantalla un mensaje de ayuda, excluyente del resto de opciones\n");
        printf("-n i: Nivel de concurrencia solicitado (uno por defecto)\n");
        printf("-d directorio: Directorio desde donde calcula el espacio utilizado (actual por defecto)\n");
        printf("salida: Archivo con lista de directorios y el espacio ocupado por los archivos regulares (salida estandar por defecto)\n");
        exit(1);
 	}

 	if (nvalue == 0){
 		nvalue = 1;
 	}

 	if (dvalue == NULL){
 		// Se trabaja con el directorio actual
 	} else {
 		// Se trabaja con el directorio dado
 		if (!es_dir(dvalue))
 		{
 			printf("El argumento %s no es un directorio.\n", dvalue);
 			exit(1);
 		}
 	}

 	// Archivo sobre el cual se imprimira el resultado
 	FILE *salida = NULL;
 	if (argc == 4){
 		//Modo read ??? deberia crearlo
 		salida = fopen(argv[optind], "r");
 	}

 	// Aqui empieza la diversion

 	return 0;

 }