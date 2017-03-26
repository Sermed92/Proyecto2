# tls

Proyecto 2: Sistemas de Operación ene-mar 2017

Sergio Medina 09-11259

Lucio Mederos 13-10856

### Uso de tls

		./tls [-h] | [-n i] [-d directorio] [salida]


Ninguna de las opciones es obligatoria.

- h: Muestra una pantalla de ayuda y **termina** el programa

- n: Se indica la cantidad "i" de hilos a crear para hacer el recorrido. **Valor por defecto:** 1

- d: Se indica el "directorio" a partir del cual se realiza el recorrido. **Valor por defecto:** Directorio actual

- salida: Se indica el nombre "salida" del archivo en el cual se imprimirá el reporte. **Valor por defecto:** Salida estándar

### Explicación

Se inicializa una cola global en la cual se guardan los nombres de todos los directorios encontrados en el recorrido

Para obtener el directorio actual, en caso de ser necesario, se usa la función **getcwd**

Para hacer el recorrido de usan las funciones **opendir** y **readdir**

El hilo maestro recorre el directorio suministrado y encola los directorios encontrados, luego crea a sus hilos trabajadores

Cada hilo trabajador desencola un directorio y lo procesa: encola directorios y retorna el numero de archivos regulares y la cantidad de bytes que estos ocupan