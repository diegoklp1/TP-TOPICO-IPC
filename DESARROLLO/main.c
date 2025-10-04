#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funciones.h"

int leerRegistroIPC(FILE *archivo, RegistroIPC *registro);

int main(void) {
    //char periodoCod[] = "749681";
    //char periodoDec[7];

    FILE *archivo_ipc = fopen("serie_ipc_divisiones.csv", "r");
    if (archivo_ipc == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    char buffer_encabezado[256];
    fgets(buffer_encabezado, sizeof(buffer_encabezado), archivo_ipc);

    RegistroIPC ra;
    int i =0;
    while (i<100) {
        leerRegistroIPC(archivo_ipc, &ra);
        printf("Registro %4d -> Descripcion: %60s, Region: %30s, Indice: %10.2f\n",ra.descripcion,ra.region,ra.indice_ipc);
        i++;
    }

    fclose(archivo_ipc);
    //decodificarFecha(periodoCod, periodoDec);
    //mostrarPalabra(periodoDec);

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
        default:  return '?';
    }
}

void decodificarFecha(const char *fechaE, char *fechaS ) {

    const char *i = fechaE;
    char *j = fechaS;

    while (*i != '\0') {
        *j++ = decodificarDigito(*i++);
    }
    *j = '\0';
}

void mostrarPalabra(const char *p) {
    while (*p)
        putchar(*p++);
    putchar('\n');
}
