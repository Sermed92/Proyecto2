/*
 * Archivo: main.c
 * Autores:
 *          Sergio Medina 09-11259
 *          Lucio Mederos 13-10856
 */

#include "cabecera.h"

int main(int argc, char** argv){

	numeroArgumentos(argc);

	int hflag = 0;
	int nvalue = 0;
	char *dvalue = NULL;
	numero_archivos = 0;
	numero_directorios = 0;
	int n;

	opterr = 0;

	while ((n = getopt(argc,argv, "hd:n:")) != -1){
		switch(n){
			case 'h':
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
					fprintf(stderr, "Opcion -%c requiere un argumactualo.\n", optopt);
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
		fprintf(stderr, "La opcion '-h' es excluyactuale del resto de las opciones\n");
		exit(1);
	}

	if (hflag == 1){
		printf("UsoDisco Usage:\n");
		printf("./UsoDisco [-h] | [-n <i>] [-d <directorio>] [salida] \n");
		printf("-h: Muestra por pantalla un mensaje de ayuda, excluyactuale del resto de opciones\n");
		printf("-n i: Nivel de concurrencia solicitado (uno por defecto)\n");
		printf("-d directorio: directorio desde donde calcula el espacio utilizado (actual por defecto)\n");
		printf("salida: Archivo con lista de directorios y el espacio ocupado por los archivos regulares (salida estandar por defecto)\n");
		exit(1);
	}

	// if (nvalue == 0){
	// 	nvalue = 1;
	// }

	if (dvalue == NULL){
		// si se toma como default el directorio actual
		char directorio_actual[BUFSIZ];
		char *cp;
		cp = getcwd(directorio_actual, sizeof(directorio_actual));
		if (cp == NULL) {
			printf("Error obteniendo el directorio actual\n");
			exit(1);
		}
		dvalue = directorio_actual;
	}

	// Se trabaja con el directorio dado
	if (!es_directorio(dvalue))
	{
		printf("El argumento '%s' no es un directorio\n", dvalue);
		exit(1);
	}

	// Archivo sobre el cual se imprimira el resultado
	if (optind < argc) {
		if ((salida = fopen(argv[optind], "w")) == NULL) {
			printf("No se pudo abrir o crear el archivo: %s\n",argv[optind]);
			exit(1);
		}
	}

	cabeza = NULL;
	cola = NULL;
	
	// Se procesa el directorio recibido en el programa principal
	procesar_directorio(dvalue);
	if (nvalue == 0) {
		// Si no se recibe ningun valor por el argumento n, se trabaja solo en el programa principal
		while (cabeza != NULL) {
			procesar_directorio(desencolar());
		}
	} else {
		pthread_t arreglo_hilos[nvalue];
		int cont = 0;
		char *d;
		while (cabeza != NULL) {
			d = desencolar();
			pthread_create(&arreglo_hilos[cont], NULL, trabajo_de_hilo, d);
			numero_hilos++;
			if (cont == nvalue) {
				cont = 0;
				for (int i = 0; i < nvalue; i++) {
					pthread_join(arreglo_hilos[i], NULL);
				}
			}
		}
		for (int i = 0; i < nvalue; i++) {
			pthread_join(arreglo_hilos[i], NULL);
		}
	}

	if (optind < argc) {
		if (fclose(salida) != 0) {
			printf("No se pudo cerrar el archivo de salida\n");
			exit(1);
		}
	}

	printf("Numero de archivos encontrado: %d\n", numero_archivos);
	printf("Numero de directorios escaneados: %d\n", numero_directorios);
	printf("Numero de hilos creados %d\n", numero_hilos);
	

	return 0;
}