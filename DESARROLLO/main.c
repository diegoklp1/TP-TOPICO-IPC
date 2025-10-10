#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

int leerRegistroIPC(FILE *archivo, RegistroIPC *registro);

int main(void) {
    int periodoDec;

    // LEER REGISTRO DE CSV
    FILE *archivo_ipc = fopen("serie_ipc_divisiones.csv", "r");
    if (archivo_ipc == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }
    char buffer_encabezado[256];
    fgets(buffer_encabezado, sizeof(buffer_encabezado), archivo_ipc);


    RegistroIPC ra;

    leerRegistroIPC(archivo_ipc, &ra);
    printf("Codigo: %s - Descripcion: %s - Indice IPC: %.2f\n - Periodo: %d\n", ra.codigo, ra.descripcion, ra.indice_ipc, ra.periodo);

    //Ejercicio 1
    periodoDec = decodificarFecha(ra.periodo);
    printf("\nPeriodo decodificado: %d",periodoDec);

    //Ejercicio 2
    convertirFecha(periodoDec,ra.fecha_convertida);
    mostrarPalabra(ra.fecha_convertida);

    //Ejercicio 3
    normalizarDescripcion(ra.descripcion);
    mostrarPalabra(ra.descripcion);

    //Ejercicio 4
    char stringIndice[6];
    sprintf(stringIndice,"%f",ra.indice_ipc);
    convertirComaAPunto(stringIndice);
    mostrarPalabra(stringIndice);


    //Ejercicio 5
    calcularMontoAjustadoPorIPC(archivo_ipc);

    //Ejercicio 6

    //Ejercicio 7

    //Ejercicio 8

    //Ejercicio 9

    fclose(archivo_ipc);


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
    limpiarCampo(registro->descripcion);
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
    limpiarCampo(registro->region);
    p_ini = p_fin + 1;

    int len = strlen(p_ini);
    while (len > 0 && (p_ini[len-1] == '\n' || p_ini[len-1] == '\r' || p_ini[len-1] == ' ')) {
        p_ini[len-1] = '\0'; len--;
    }

    if (p_ini[0] == '"' && p_ini[len-1] == '"') {
        p_ini[len-1] = '\0'; p_ini++;
    }

    registro->periodo = atoi(p_ini);

    return 1;
}

void limpiarCampo(char *campo) {
    int len = strlen(campo);

    while (len > 0 && (campo[len-1] == '\n' || campo[len-1] == '\r' || campo[len-1] == ' ')) {
        campo[len-1] = '\0';
        len--;
    }

    if (len > 1 && campo[0] == '"' && campo[len-1] == '"') {
        campo[len-1] = '\0';
        memmove(campo, campo + 1, strlen(campo + 1) + 1);
    }
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
        case '5': return '9';
        default:  return '?';
    }
}

int decodificarFecha(int fecha_codif) {
    char stringFechaCodif[16];
    char stringFechaDecodif[16];

    sprintf(stringFechaCodif, "%d", fecha_codif);

    const char *i = stringFechaCodif;
    char *j = stringFechaDecodif;

    while (*i != '\0') {
        *j++ = decodificarDigito(*i++);
    }
    *j = '\0';

    printf("Fecha decodificada -> %s\n", stringFechaDecodif);
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

// 3: Normalizar descripción
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

// 4: Convertir coma a punto
void convertirComaAPunto(char *p) {
    while (*p) {
        if (*p == ',') {
            *p = '.';
        }
        *p++;
    }
}

// 5: Cálculo del monto ajustado por IPC
void calcularMontoAjustadoPorIPC(FILE *archivo_ipc) {
    float monto;
    char region[10];
    int fechaInicio, fechaFin;
    double ipcInicio= 0, ipcFin= 0;

    //Solicitamos los datos necesarios
    solicitarMonto(&monto);
    solicitarRegion(region);
    solicitarFecha(&fechaInicio);
    solicitarFecha(&fechaFin);

    RegistroIPC ra;

    //Buscamos los IPCs
    buscarIPCs(archivo_ipc, region, fechaInicio, fechaFin, &ipcInicio, &ipcFin, ra);


    //Informamos los resultados
    if (ipcInicio == 0 || ipcFin == 0) {
        printf("No se encontraron los datos del IPC");
    }else {
        printf("Monto inicial: %2.f | Monto ajustado: %2.f\n", monto, monto * (ipcFin / ipcInicio));

        printf("Variacion porcentual: %2.f\n", (ipcFin / ipcInicio - 1) * 100);
    }

    //Regresamos el archivo al inicio
    rewind(archivo_ipc);
}

void solicitarMonto(float *monto) {
    printf("Ingrese el monto a evaluar:");
    scanf("%f", monto);
    fflush(stdin);
}

void solicitarRegion(char *region) {
    int idRegion;
    do {
        printf("Ingrese la region:\n"
           "1. Nacional\n"
           "2. GBA\n"
           "3. Pampeana\n"
           "4. Cuyo\n"
           "5. Noroeste\n"
           "6. Noreste\n"
           "7. Patagonia");
        scanf("%d", &idRegion);
        fflush(stdin);
        if (idRegion < 1 || idRegion > 7)
            printf("Valor invalido\n");
    }while (idRegion < 1 || idRegion > 7);

    switch (idRegion) {
        case 1: strcpy(region, "Nacional"); break;
        case 2: strcpy(region, "GBA"); break;
        case 3: strcpy(region, "Pampeana"); break;
        case 4: strcpy(region, "Cuyo"); break;
        case 5: strcpy(region, "Noroeste"); break;
        case 6: strcpy(region, "Noreste"); break;
        case 7: strcpy(region, "Patagonia"); break;
        default: strcpy(region, "Invalido"); break;
    }
}

void solicitarFecha(int *fecha) {
    printf("Ingrese la fecha con formato aaaamm: ");
    scanf("%d", fecha);
}

void buscarIPCs(FILE *archivo_ipc, char region[10], int fechaInicio, int fechaFin, double *ipcInicio, double *ipcFin, RegistroIPC ra) {
    while (leerRegistroIPC(archivo_ipc, &ra) && (*ipcInicio == 0 || *ipcFin == 0)) {
        if (strcmpi(ra.descripcion, "nivel general") == 0 && strcmpi(ra.region, region) == 0) {
            if (fechaInicio == decodificarFecha(ra.periodo)) {
                *ipcInicio = ra.indice_ipc;
            }else if (fechaFin == decodificarFecha(ra.periodo)) {
                *ipcFin = ra.indice_ipc;
            }
        }
    }
}