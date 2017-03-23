/*
 * Archivo: main.c
 * Autores:
 *          Sergio Medina 09-11259
 *          Lucio Mederos 13-10856
 */

#include "libreria.h"

int main(int argc, char** argv){

    // Se verifican los argumentos
	numeroArgumentos(argc);

	int hflag = 0;
	int n_hilos = 0;
	char *directorio = NULL;
	int n;

	opterr = 0;

    // Se procede a procesar las opciones
	while ((n = getopt(argc,argv, "hd:n:")) != -1){
		switch(n){
			case 'h':
				hflag = 1;
				break;
			case 'd':
	            	directorio = optarg;
				break;
			case 'n':
				n_hilos = atoi(optarg);
				break;
			case '?':
				if (optopt == 'n'){
					fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
				} else if (isprint(optopt)){
					fprintf(stderr, "Opcion desconocida '-%c'.\n", optopt);
				} else {
					fprintf(stderr, "Caracter desconocido '\\x%x'.\n", optopt);
				}
				return 1;
			default:
				abort();
		}
	}

	// Si se pasa la opcion h no deben haber mas opciones
    opcion_help(hflag, argc);

	asignar_directorio(&directorio);

	salida = crear_salida(optind, argc, argv);
	cabeza = NULL;
	cola = NULL;

	// Se procesa el directorio recibido en el programa principal
	procesar_directorio(directorio);
	crear_hilos(n_hilos);
    cerrar_salida(optind, argc);
	return 0;
}
