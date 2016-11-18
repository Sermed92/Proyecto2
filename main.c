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

//printf("Cantidad de hilos: %i\n", nvalue);
	if (nvalue == 0){
		nvalue = 1;
	}

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

	agregar_slash(dvalue);

	// Se trabaja con el directorio dado
	if (!es_directorio(dvalue))
	{
		printf("El argumento '%s' no es un directorio\n", dvalue);
		exit(1);
	}
	else {
		printf("El argumento '%s' si es un directorio\n", dvalue);
	}

	// Archivo sobre el cual se imprimira el resultado
	//salida = NULL;
	// if (argc == 4){
	// 	//Modo write lo crea
	// 	salida = fopen(argv[optind], "w");
	// 	printf("abro archivo %s/n",argv[optind]);
	// }

	// Aqui empieza la diversion
	cabeza = NULL;
	cola = NULL;
	//dvalue = agregar_slash(dvalue);
	printf("Trabajo con: %s\n",dvalue);
	
	procesar_directorio(dvalue);
	//printf("cabeza%s\n",cabeza->directorio);
	while (cabeza != NULL) {
		procesar_directorio(desencolar());
	}

	return 0;

}