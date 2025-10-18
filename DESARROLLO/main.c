#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funciones.h"

int main(void) {
    
    // LEER REGISTRO DE CSV
    FILE *archivo_ipc = fopen("serie_ipc_divisiones.csv", "r");
    if (archivo_ipc == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }
    //Variables de prueba
    int periodoDec;
    char buffer_encabezado[256];

    //Lectura de registro de csv
    fgets(buffer_encabezado, sizeof(buffer_encabezado), archivo_ipc);
    RegistroIPC ra;
    printf("\nPRUEBA DE LECTURA CSV");
    leerRegistroIPC("serie_ipc_divisiones.csv", &ra);
    printf("\nCodigo: %s - Descripcion: %s - Indice IPC: %.2f - Periodo: %d", ra.codigo, ra.descripcion, ra.indice_ipc, ra.periodo);

    //Ejercicio 1
    printf("\nEJ1 --- ");
    printf("\nBEFORE:%d",ra.periodo);
    periodoDec = decodificarFecha(ra.periodo);
    printf("\nAFTER:\nPeriodo decodificado: %d",periodoDec);
    //Ejercicio 2
    printf("\nEJ2 --- ");
    convertirFecha(periodoDec,ra.fecha_convertida);
    mostrarPalabra(ra.fecha_convertida);
    //Ejercicio 3
    printf("\nEJ3 --- ");
    normalizarDescripcion(ra.descripcion);
    mostrarPalabra(ra.descripcion);
    //Ejercicio 4
    printf("\nEJ4 --- ");
    char stringIndice[6];
    sprintf(stringIndice,"%f",ra.indice_ipc);
    convertirComaAPunto(stringIndice);
    mostrarPalabra(stringIndice);
    //Ejercicio 5
    printf("\nEJ5 --- ");
    // Se puede probar monto=15000, region=GBA, fechas : 201701 - 201712 , entonces el ipc1: 101.3130, ipc2: 125.0392 => variacion porcentual
    calcularMontoAjustadoPorIPC(archivo_ipc);
    //Ejercicio 6
    printf("\nEJ6 --- ");
    Vector vector;
    vectorCrear(&vector, sizeof(Clasificacion));
    calcularIPCPorGrupos(archivo_ipc, &vector);
    //Ejercicio 7

    //Ejercicio 8

    //Ejercicio 9
    calcularAjusteAlquiler();

    fclose(archivo_ipc);
    return 0;
}
bool leerRegistroIPC(char* nomArch, RegistroIPC* reg)
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

    fgets(buffer, sizeof(buffer), fpArch);

    if (trozarLineaDivisiones(buffer, reg))
    {
        //mostrarRegistroIPC(reg);
    }
    else
    {
        //fprintf(stderr, "ADVERTENCIA: Línea con formato incorrecto omitida: %s", buffer);
        puts("Linea Incorrecta");
    }

    fclose(fpArch);
    return true;
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
        if (trozarLineaDivisiones(buffer, &reg))
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

bool trozarLineaDivisiones(char buffer[], RegistroIPC *registro)
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
bool trozarLineaAperturas(char buffer[], RegistroIPC *registro)
{
    char* pos;
    pos = strchr(buffer, '\n');
    if (!pos) {
        return false;
    }

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

    //periodo
    pos = strrchr(buffer, ';');
    if (!pos)
        return false;
    limpiarCampo(pos+1);
    registro->periodo = atoi(pos + 1);
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

bool vectorCrear(Vector* vector, size_t tamanioElemento)
{
    vector->vector = malloc(tamanioElemento * 10);

    if(!(vector->vector)){
        vector->capacidad = 0;
        return false;
    }

    vector->tamanioElemento = tamanioElemento;
    vector->cantidadElementos = 0;
    vector->capacidad = 10;

    return true;
}

void vectorDestruir(Vector* vector)
{
    free(vector->vector);
    vector->vector = NULL;
    vector->tamanioElemento = 0;
    vector->cantidadElementos = 0;
    vector->capacidad = 0;
}


bool redimensionarVector(Vector* vector, size_t capacidad)
{
    int* vectorTemporal = realloc(vector->vector, capacidad * vector->tamanioElemento);

    if(!(vectorTemporal)){
        return false;
    }

    printf("Redimension de %d a %d\n", vector->capacidad, capacidad);

    vector->vector = vectorTemporal;
    vector->capacidad = capacidad;

    return true;
}

int vectorInsertar(Vector* vector, void* elemento)
{
    if(vector->cantidadElementos == vector->capacidad){
        if(!redimensionarVector(vector, vector->capacidad * 2)){
            return 0;
        }
    }

    memcpy((char*)vector->vector + vector->cantidadElementos * vector->tamanioElemento,
           elemento, vector->tamanioElemento);
    vector->cantidadElementos++;

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
        p++;
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
        if (trozarLineaDivisiones(buffer, &ra) && strcmpi(ra.descripcion, "nivel general") == 0 && strcmpi(ra.region, region) == 0) {
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
// 6
void calcularIPCPorGrupos(FILE *archivo_ipc, Vector* grupos) {
    char buffer[500];
    fgets(buffer, sizeof(buffer), archivo_ipc);
    RegistroIPC ra;
    Clasificacion clasificacion;
    while (fgets(buffer, sizeof(buffer), archivo_ipc)) {
        if (trozarLineaDivisiones(buffer, &ra)) {
            if (strcmpi(ra.region, "nacional") == 0) {
                clasificacion.periodo = decodificarFecha(ra.periodo);
                convertirFecha(clasificacion.periodo, clasificacion.fecha);
                strcpy(clasificacion.descripcion, ra.descripcion);
                clasificacion.indice_ipc = ra.indice_ipc;
                strcpy(clasificacion.region, ra.region);
                clasificarGrupo(ra.descripcion, clasificacion.grupo);

                vectorInsertar(grupos, &clasificacion);
            }
        }
    }
    mostrarPromedios(grupos);

    rewind(archivo_ipc);
}

void clasificarGrupo(const char* descripcion, char *grupo) {
    if (strcmpi(descripcion, "alimentos y bebidas no alcohólicas") == 0 ||
        strcmpi(descripcion, "bebidas alcohólicas y tabaco") == 0 ||
        strcmpi(descripcion, "prendas de vestir y calzado") == 0 ||
        strcmpi(descripcion, "bienes y servicios varios") == 0 ||
        strcmpi(descripcion, "equipamiento y mantenimiento del hogar") == 0) {
        strcpy(grupo, "Bienes");
    } else if (strcmpi(descripcion, "recreación y cultura") == 0 ||
        strcmpi(descripcion, "restaurantes y hoteles") == 0 ||
        strcmpi(descripcion, "salud") == 0 ||
        strcmpi(descripcion, "transporte") == 0 ||
        strcmpi(descripcion, "educación") == 0 ||
        strcmpi(descripcion, "comunicación") == 0 ||
        strcmpi(descripcion, "vivienda, agua, electricidad, gas y otros combustibles") == 0) {
        strcpy(grupo, "Servicios");
    } else {
        strcpy(grupo, "Otro");
    }
}

void mostrarPromedios(Vector* grupos) {
    int periodoActual = 0;
    double sumaBienes = 0, sumaServicios = 0;
    int cantBienes = 0, cantServicios = 0;

    Clasificacion* clasificacion = grupos->vector;

    for (size_t i = 0; i < grupos->cantidadElementos; i++) {
        if (periodoActual != clasificacion[i].periodo && periodoActual != 0) {
            printf("\nPeriodo %d -> Bienes: %.2f | Servicios: %.2f",
                   periodoActual,
                   cantBienes ? sumaBienes / cantBienes : 0,
                   cantServicios ? sumaServicios / cantServicios : 0);

            sumaBienes = sumaServicios = 0;
            cantBienes = cantServicios = 0;
        }

        periodoActual = clasificacion[i].periodo;

        if (strcmpi(clasificacion[i].grupo, "Bienes") == 0) {
            sumaBienes += clasificacion[i].indice_ipc;
            cantBienes++;
        } else if (strcmpi(clasificacion[i].grupo, "Servicios") == 0) {
            sumaServicios += clasificacion[i].indice_ipc;
            cantServicios++;
        }
    }

    if (cantBienes + cantServicios > 0) {
        printf("\nPeriodo %d -> Bienes: %.2f | Servicios: %.2f\n",
               periodoActual,
               cantBienes ? sumaBienes / cantBienes : 0,
               cantServicios ? sumaServicios / cantServicios : 0);
    }
}

// 7

// 8
// Se reutiliza la función del punto 4

// 9
void calcularAjusteAlquiler() {
    float monto;
    char region[10];
    int fechaInicio;
    double ipcInicio = 0, ipcMax = 0;
    int fechaMax = 0;

    solicitarMonto(&monto);
    solicitarRegion(region);
    printf("--- Ingrese el Periodo de Inicio de Contrato ---\n");
    solicitarFecha(&fechaInicio);

    FILE* archivo_aperturas = fopen(FILENAME_APERTURAS, "r");
    if (archivo_aperturas == NULL) {
        perror("Error al abrir el archivo serie_ipc_aperturas.csv");
        return;
    }

    char buffer[500];
    RegistroIPC ra;
    RegistroIPC registros_encontrados[MAX_MESES_REPORTE];
    int count = 0;
    int encontradoInicio = 0;

    fgets(buffer, sizeof(buffer), archivo_aperturas); // Saltar encabezado

    while (fgets(buffer, sizeof(buffer), archivo_aperturas)) {
        if (trozarLineaAperturas(buffer, &ra)) {
            if (strcmpi(ra.descripcion, FILTRO_DESCRIPCION_ALQUILER) == 0 && strcmpi(ra.region, region) == 0) {

                if (ra.periodo == fechaInicio) {
                    ipcInicio = ra.indice_ipc;
                    encontradoInicio = 1;
                }

                if (encontradoInicio && count < MAX_MESES_REPORTE) {
                    registros_encontrados[count++] = ra;

                
                    if (ra.periodo > fechaMax) {
                        fechaMax = ra.periodo;
                        ipcMax = ra.indice_ipc;
                    }
                }
            }
        }
    }
    fclose(archivo_aperturas); 

    if (ipcInicio == 0 || count == 0) {
        printf("Error: No se pudo encontrar el indice de inicio para la region y fecha dadas.\n");
        return;
    }

    double montoAjustadoTotal = monto * (ipcMax / ipcInicio);
    double variacionTotal = (ipcMax / ipcInicio - 1) * 100;

    printf("\n--- Calculadora de Alquileres ---\n");
    printf("Monto inicial:       $ %.2f\n", monto);
    printf("Monto ajustado:      $ %.2f\n", montoAjustadoTotal);
    printf("Variacion porcentual:  %.2f %%\n", variacionTotal);

    FILE* archivo_binario = fopen(ARCHIVO_BINARIO_SALIDA, "wb");
    if (archivo_binario == NULL) {
        perror("Error al crear archivo binario de salida");
        return;
    }

    printf("\n--- Detalle Mes a Mes ---\n");
    printf("-----------------------------------------------------------\n");
    printf("%-10s %-12s %-12s %-15s\n", "Periodo", "Indice", "Variacion %", "Monto ajustado");
    printf("-----------------------------------------------------------\n");

    FilaTablaAlquiler fila;

    //Iterar sobre los registros guardados para generar la tabla
    for (int i = 0; i < count; i++) {
        ra = registros_encontrados[i];

        // Llenar la struct 'fila' para la tabla
        sprintf(fila.periodo, "%d-%02d", ra.periodo / 100, ra.periodo % 100);
        fila.indice = ra.indice_ipc;
        //variación y monto contra el índice INICIAL 
        fila.variacionPct = (ra.indice_ipc / ipcInicio - 1) * 100;
        fila.montoAjustado = monto * (ra.indice_ipc / ipcInicio);

        printf("%-10s %-12.2f %-12.2f %-15.2f\n",
               fila.periodo,
               fila.indice,
               fila.variacionPct,
               fila.montoAjustado);

        fwrite(&fila, sizeof(FilaTablaAlquiler), 1, archivo_binario);
    }

    fclose(archivo_binario); // Cerramos el archivo binario

    //Leer y mostrar el archivo binario
    leerMostrarTablaBinario(ARCHIVO_BINARIO_SALIDA);
}
void leerMostrarTablaBinario(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        printf("\nError al abrir el archivo binario '%s' para lectura.\n", nombreArchivo);
        return;
    }

    printf("\n\n--- Mostrando datos leidos desde '%s' ---\n", nombreArchivo);
    printf("-----------------------------------------------------------\n");
    printf("%-10s %-12s %-12s %-15s\n", "Periodo", "Indice", "Variacion %", "Monto ajustado");
    printf("-----------------------------------------------------------\n");

    FilaTablaAlquiler fila;
    // Lee registros del tamaño de la struct FilaTablaAlquiler, 1 a la vez
    while (fread(&fila, sizeof(FilaTablaAlquiler), 1, archivo) == 1) {
        printf("%-10s %-12.2f %-12.2f %-15.2f\n",
               fila.periodo,
               fila.indice,
               fila.variacionPct,
               fila.montoAjustado);
    }
    fclose(archivo);
}

