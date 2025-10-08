#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

int leerRegistroIPC(FILE *archivo, RegistroIPC *registro);

int main(void) {
    int periodoCod = 979547;
    int periodoDec;
    // LEER REGISTRO DE CSV
    /*
    FILE *archivo_ipc = fopen("serie_ipc_divisiones.csv", "r");
    if (archivo_ipc == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }
    char buffer_encabezado[256];
    fgets(buffer_encabezado, sizeof(buffer_encabezado), archivo_ipc);
    */

    RegistroIPC ra;

    /*fclose(archivo_ipc);*/


    // Funciones
    //1
    periodoDec = decodificarFecha(periodoCod);
    printf("\n%d",periodoDec);
    //mostrarPalabra(periodoDec);
    //2
    convertirFecha(periodoDec,ra.fecha_convertida);
    mostrarPalabra(ra.fecha_convertida);
    char palabra[20];
    strcpy(palabra,"HOLA MUNDO");
    normalizarDescripcion(palabra);
    mostrarPalabra(palabra);


    double numero = 123.456;
    char texto[50];

    sprintf(texto, "%.3f", numero);
    convertirComaAPunto(texto);
    printf("Texto: %s\n", texto);
    return 0;
}

int leerRegistroIPC(FILE *archivo, RegistroIPC *registro) {
    char linea[256];

    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        return 0;
    }

    char *p_ini = linea;
    char *p_fin;
    int longitud_campo;

    // codigo
    p_fin = strchr(p_ini, ';');
    if (!p_fin) return 0;
    longitud_campo = p_fin - p_ini;
    strncpy(registro->codigo, p_ini, longitud_campo);
    registro->codigo[longitud_campo] = '\0';
    p_ini = p_fin + 1;

    // descripcion
    p_fin = strchr(p_ini, ';');
    if (!p_fin) return 0;
    longitud_campo = p_fin - p_ini;
    strncpy(registro->descripcion, p_ini, longitud_campo);
    registro->descripcion[longitud_campo] = '\0';
    p_ini = p_fin + 1;

    // clasificador
    p_fin = strchr(p_ini, ';');
    if (!p_fin) return 0;
    longitud_campo = p_fin - p_ini;
    strncpy(registro->clasificador, p_ini, longitud_campo);
    registro->clasificador[longitud_campo] = '\0';
    p_ini = p_fin + 1;

    char buffer_temporal[64];

    // indice
    p_fin = strchr(p_ini, ';');
    if (!p_fin) return 0;
    longitud_campo = p_fin - p_ini;
    strncpy(buffer_temporal, p_ini, longitud_campo);
    buffer_temporal[longitud_campo] = '\0';
    registro->indice_ipc = atof(buffer_temporal);
    p_ini = p_fin + 1;

    // variacion mensual
    p_fin = strchr(p_ini, ';');
    if (!p_fin) return 0;
    longitud_campo = p_fin - p_ini;
    strncpy(buffer_temporal, p_ini, longitud_campo);
    buffer_temporal[longitud_campo] = '\0';
    registro->variacion_mensual = atof(buffer_temporal);
    p_ini = p_fin + 1;

    // variacion interanual
    p_fin = strchr(p_ini, ';');
    if (!p_fin) return 0;
    longitud_campo = p_fin - p_ini;
    strncpy(buffer_temporal, p_ini, longitud_campo);
    buffer_temporal[longitud_campo] = '\0';
    registro->variacion_interanual = atof(buffer_temporal);
    p_ini = p_fin + 1;

    // region
    p_fin = strchr(p_ini, ';');
    if (!p_fin) return 0;
    longitud_campo = p_fin - p_ini;
    strncpy(registro->region, p_ini, longitud_campo);
    registro->region[longitud_campo] = '\0';
    p_ini = p_fin + 1;

    // periodo
    registro->periodo = atoi(p_ini);

    return 1;
}


// 1 : DECODIFICAR FECHA

char decodificarDigito(char c) {
    switch (c) {
        case '7': return '0';
        case '4': return '1';
        case '9': return '2';
        case '8': return '3';
        case '0': return '4';
        case '6': return '5';
        case '1': return '6';
        case '3': return '7';
        case '2': return '8';
        case '5': return '5';
        default:  return '?';
    }
}

int decodificarFecha(int fecha_codif) {

    char stringFechaCodif[10];
    char stringFechaDecodif[10];

    sprintf(stringFechaCodif,"%d",fecha_codif);

    const char *i = stringFechaCodif;
    char *j = stringFechaDecodif;

    while (*i != '\0') {
        *j++ = decodificarDigito(*i++);
    }
    *j = '\0';

    return atoi(stringFechaDecodif);
}

// 2 : CONVERTIR FECHA DE NUMERO A CADENA

void convertirFecha(int fecha,char* fechaTexto){
    char* meses[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio","Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    int anio = fecha / 100;
    int mes = fecha % 100;

    if (mes >= 1 && mes <= 12) {
        sprintf(fechaTexto, "%s - %d \0", meses[mes], anio);
    } else {
        sprintf(fechaTexto, "Fecha Invalida");
    }
}

void mostrarPalabra(const char *p) {
    printf("\n");
    while (*p)
        putchar(*p++);
    putchar('\n');
}


void normalizarDescripcion(char *p) {
    if (*p) {
        *p = toupper(*p);
        p++;
    }

    while (*p) {
        *p = tolower(*p);
        p++;
    }
}

void convertirComaAPunto(char *p) {
    while (*p) {
        if (*p == ',') {
            *p = '.';
        }
        *p++;
    }
}