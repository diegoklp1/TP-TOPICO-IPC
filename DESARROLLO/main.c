#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funciones.h"

int main(void) {
    int periodoDec;

    // LEER REGISTRO DE CSV
    FILE *archivo_ipc = fopen("serie_ipc_divisiones.csv", "r");
    if (archivo_ipc == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    //Ejercicio 5
    calcularMontoAjustadoPorIPC(archivo_ipc);

    //Ejercicio 6

    //Ejercicio 7

    //Ejercicio 8

    //Ejercicio 9

    fclose(archivo_ipc);


    return 0;
}

bool leerArchivoCompletoIPC(char* nomArch)
{
    FILE* fpArch = fopen(nomArch, "r");
    if (!fpArch)
    {
        puts("ERROR: No se pudo abrir el archivo.");
        return false;
    }

    char buffer[500];
    // Se salta la línea del encabezado
    fgets(buffer, sizeof(buffer), fpArch);

    RegistroIPC reg;
    while (fgets(buffer, sizeof(buffer), fpArch))
    {
        if (trozarLineaIPC(buffer, &reg))
        {
            //mostrarRegistroIPC(reg);
        }
        else
        {
            //fprintf(stderr, "ADVERTENCIA: Línea con formato incorrecto omitida: %s", buffer);
            puts("Linea Incorrecta");
        }
    }

    fclose(fpArch);
    return true;
}

bool trozarLineaIPC(char buffer[], RegistroIPC *registro)
{
    char* pos;
    pos = strchr(buffer, '\n');
    if (!pos) {
        return false;
    }

    *pos = '\0';
    //periodo
    pos = strrchr(buffer, ';');
    if (!pos)
        return false;
    limpiarCampo(pos+1);
    registro->periodo = atoi(pos + 1);

    *pos = '\0';

    //region
    pos = strrchr(buffer, ';');
    if (!pos) return false;
    limpiarCampo(pos+1);
    strcpy(registro->region, pos + 1);
    *pos = '\0';

    //variacion_interanual
    pos = strrchr(buffer, ';');
    if (!pos) return false;
    registro->variacion_interanual = atof(pos + 1);
    *pos = '\0';

    //variacion_mensual
    pos = strrchr(buffer, ';');
    if (!pos) return false;
    registro->variacion_mensual = atof(pos + 1);
    *pos = '\0';

    //indice_ipc
    pos = strrchr(buffer, ';');
    if (!pos) return false;
    registro->indice_ipc = atof(pos + 1);
    *pos = '\0';

    //clasificador
    pos = strrchr(buffer, ';');
    if (!pos) return false;
    strcpy(registro->clasificador, pos + 1);
    *pos = '\0';

    //descripcion
    pos = strrchr(buffer, ';');
    if (!pos) return false;
    limpiarCampo(pos+1);
    strcpy(registro->descripcion, pos + 1);
    *pos = '\0';

    //codigo
    strcpy(registro->codigo, buffer);

    return true;
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
        sprintf(fechaTexto, "%s - %d", meses[mes], anio);
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
    char buffer[500];
    // Se salta la línea del encabezado
    fgets(buffer, sizeof(buffer), archivo_ipc);

    //Buscamos los IPCs
    while (fgets(buffer, sizeof(buffer), archivo_ipc) && (ipcInicio == 0 || ipcFin == 0)) {
        if (trozarLineaIPC(buffer, &ra) && strcmpi(ra.descripcion, "nivel general") == 0 && strcmpi(ra.region, region) == 0) {
            if (fechaInicio == decodificarFecha(ra.periodo))
                ipcInicio = ra.indice_ipc;
            else if (fechaFin == decodificarFecha(ra.periodo))
                ipcFin = ra.indice_ipc;
        }
    }

    //Informamos los resultados
    if (ipcInicio == 0 || ipcFin == 0) {
        printf("No se encontraron los datos del IPC");
    }else {
        printf("Monto inicial: %.2f | Monto ajustado: %.2f\n", monto, monto * (ipcFin / ipcInicio));

        printf("Variacion porcentual: %.2f\n", (ipcFin / ipcInicio - 1) * 100);
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