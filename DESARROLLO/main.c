#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funciones.h"

#define MAX_LINEA 512
#define TAM_INI 100
#define FACT_INCR 1.5
int main(void) {

    // LEER REGISTRO DE CSV
    FILE* archivo_ipc = fopen("serie_ipc_divisiones.csv", "r");
    if (!archivo_ipc) {
        puts("Error al abrir el archivo");
        return 1;
    }
    FILE* archivo_ipc_act = fopen("arch_temporal.csv", "w");
    if (!archivo_ipc_act) {
        fclose(archivo_ipc);
        puts("Error al crear el archivo actualizado");
        return 1;
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo_ipc);
    fprintf(archivo_ipc_act, "%s", linea);
    RegistroIPC reg;
    //int lim = 0;
    while (fgets(linea, sizeof(linea), archivo_ipc))
    {
        //lim++;
        trozarLineaDivisiones(linea,&reg);
        //EJ1
        decodificarFecha(reg.periodo);
        //EJ2
        convertirFecha(reg.periodo);
        //EJ3
        normalizarDescripcion(reg.descripcion);
        //EJ4
        convertirComaAPunto(reg.indice_ipc);

        fprintf(archivo_ipc_act,"%s;%s;%s;%s;%.2f;%.2f;%s;%s\n",reg.codigo,reg.descripcion,reg.clasificador,reg.indice_ipc,reg.variacion_mensual,reg.variacion_interanual,reg.region,reg.periodo);
    }

    fclose(archivo_ipc);
    fclose(archivo_ipc_act);

    remove("serie_ipc_divisiones.csv");
    rename("arch_temporal.csv","serie_ipc_divisiones.csv");

    
    //Ejercicio 5
    printf("\nEJ5 --- ");
    // Se puede probar monto=15000, region=GBA, fechas : 201701 - 201712
    //calcularMontoAjustadoPorIPC("serie_ipc_divisiones.csv");
    
    //Ejercicio 6
    printf("\nEJ6 --- ");
    Vector vector;
    vectorCrear(&vector, sizeof(Clasificacion));
    calcularIPCPorGrupos("serie_ipc_divisiones.csv", &vector);
    free(vector.vector);
    /*
    //Ejercicio 7
     septimoEjercicio("serie_ipc_aperturas.csv");

    //TO-DO

    //Ejercicio 8
    //Se reutiliza la funcion del punto 4

    //Ejercicio 9
    calcularAjusteAlquiler();
    */

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

    char linea[500];
    // Se salta la línea del encabezado
    fgets(linea, sizeof(linea), fpArch);

    fgets(linea, sizeof(linea), fpArch);

    if (trozarLineaDivisiones(linea, reg))
    {
        //mostrarRegistroIPC(reg);
    }
    else
    {
        //fprintf(stderr, "ADVERTENCIA: Línea con formato incorrecto omitida: %s", linea);
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

    char linea[500];
    // Se salta la línea del encabezado
    fgets(linea, sizeof(linea), fpArch);

    RegistroIPC reg;
    while (fgets(linea, sizeof(linea), fpArch))
    {
        if (trozarLineaDivisiones(linea, &reg))
        {
            //mostrarRegistroIPC(reg);
        }
        else
        {
            //fprintf(stderr, "ADVERTENCIA: Línea con formato incorrecto omitida: %s", linea);
            puts("Linea Incorrecta");
        }
    }

    fclose(fpArch);
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
bool trozarLineaDivisiones(char linea[], RegistroIPC *registro)
{
    char* pos;
    pos = strchr(linea, '\n');
    if (!pos) {
        return false;
    }

    *pos = '\0';
    //periodo
    pos = strrchr(linea, ';');
    if (!pos)
        return false;
    limpiarCampo(pos+1);
    strcpy(registro->periodo, pos + 1);

    *pos = '\0';
    //region
    pos = strrchr(linea, ';');
    if (!pos) return false;
    limpiarCampo(pos+1);
    strcpy(registro->region, pos + 1);
    *pos = '\0';

    //variacion_interanual
    pos = strrchr(linea, ';');
    if (!pos) return false;
    convertirComaAPunto(pos + 1);
    registro->variacion_interanual = atof(pos + 1);
    *pos = '\0';

    //variacion_mensual
    pos = strrchr(linea, ';');
    if (!pos) return false;
    convertirComaAPunto(pos + 1);
    registro->variacion_mensual = atof(pos + 1);
    *pos = '\0';

    //indice_ipc
    pos = strrchr(linea, ';');
    if (!pos) return false;
    limpiarCampo(pos+1);
    strcpy(registro->indice_ipc, pos + 1);
    *pos = '\0';

    //clasificador
    pos = strrchr(linea, ';');
    if (!pos) return false;
    limpiarCampo(pos+1);
    strcpy(registro->clasificador, pos + 1);
    *pos = '\0';

    //descripcion
    pos = strrchr(linea, ';');
    if (!pos) return false;
    limpiarCampo(pos+1);
    strcpy(registro->descripcion, pos + 1);
    *pos = '\0';

    //codigo
    limpiarCampo(linea);
    strcpy(registro->codigo, linea);

    return true;
}
bool trozarLineaAperturas(char linea[], RegistroIPC *registro)
{
    char* pos;
    pos = strchr(linea, '\n');
    if (!pos) {
        return false;
    }

    *pos = '\0';
    //region
    pos = strrchr(linea, ';');
    if (!pos) return false;
    //limpiarCampo(pos+1);
    strcpy(registro->region, pos + 1);
    *pos = '\0';

    //variacion_interanual
    pos = strrchr(linea, ';');
    if (!pos) return false;
    registro->variacion_interanual = atof(pos + 1);
    *pos = '\0';

    //variacion_mensual
    pos = strrchr(linea, ';');
    if (!pos) return false;
    registro->variacion_mensual = atof(pos + 1);
    *pos = '\0';

    //indice_ipc
    pos = strrchr(linea, ';');
    if (!pos) return false;
    //convertirComaAPunto(pos+1);
    strcpy(registro->indice_ipc, pos + 1);
    *pos = '\0';

    //periodo
    pos = strrchr(linea, ';');
    if (!pos)
        return false;
    //limpiarCampo(pos+1);
    strcpy(registro->periodo, pos + 1);
    *pos = '\0';

    //clasificador
    pos = strrchr(linea, ';');
    if (!pos) return false;
    strcpy(registro->clasificador, pos + 1);
    *pos = '\0';

    //descripcion
    pos = strrchr(linea, ';');
    if (!pos) return false;
    //limpiarCampo(pos+1);
    strcpy(registro->descripcion, pos + 1);
    *pos = '\0';

    //codigo
    strcpy(registro->codigo, linea);

    return true;
}

/*
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
*/
bool vectorCrear(Vector* vector, size_t tamanioElemento)
{
    vector->vector = malloc(tamanioElemento * TAM_INI);

    if(!(vector->vector)){
        vector->capacidad = 0;
        return false;
    }

    vector->tamanioElemento = tamanioElemento;
    vector->cantidadElementos = 0;
    vector->capacidad = TAM_INI;

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


static bool redimensionarVector(Vector* vector, double factorInc)
{
    size_t nuevaCap = (size_t)vector->capacidad*factorInc;
    void* vectorTemporal = realloc(vector->vector,nuevaCap* vector->tamanioElemento);

    if(!(vectorTemporal)){
        return false;
    }

    printf("Redimension de %d a %d\n", vector->capacidad, nuevaCap);

    vector->vector = vectorTemporal;
    vector->capacidad = nuevaCap;

    return true;
}

int vectorInsertar(Vector* vector, void* elemento)
{
    if(vector->cantidadElementos == vector->capacidad){
        if(!redimensionarVector(vector, FACT_INCR)){
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
        default:  return 'c';
    }
}

void decodificarFecha(char* fecha_codif) {

    char* i = fecha_codif;
    while (isdigit(*i)) {
        *i = decodificarDigito(*i);
        i++;
    }
}

// 2 : CONVERTIR FECHA DE NUMERO A CADENA

void convertirFecha(char* fechaText){
    char* meses[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio","Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    int fecha = atoi(fechaText);

    int anio = fecha / 100;
    int mes = fecha % 100;

    if (mes >= 1 && mes <= 12) {
        sprintf(fechaText, "%s - %d", meses[mes], anio);
    } else {
        sprintf(fechaText, "Fecha Invalida");
    }
}

void mostrarPalabra(const char *p) {
    printf("\n");
    while (*p)
        putchar(*p++);
    putchar('\n');
}

// 3: Normalizar descripción
void normalizarDescripcion(char *pLec) {

    char* pEscr = pLec;

    while (*pLec ==' ')
    {
        pLec++;
    }

    if (*pLec) {
        *pEscr = toupper(*pLec);
        pLec++;
        pEscr++;
    }

    while (*pLec) {
        *pEscr = tolower(*pLec);
        pEscr++;
        pLec++;
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
void calcularMontoAjustadoPorIPC(const char* nomArchivoIpc) {
    FILE* fPArchIpc = fopen(nomArchivoIpc,"r");
    if(!fPArchIpc)
    {
        puts("ERROR PARA ABRIR ARCHIVO DE DIVISIONES");
        return;
    }
    
    float monto;
    char region[10];
    int fechaInicio, fechaFin;
    double ipcInicio= 0, ipcFin= 0;

    //Solicitamos los datos necesarios
    solicitarMonto(&monto);
    solicitarRegion(region);
    solicitarFecha(&fechaInicio);
    solicitarFecha(&fechaFin);

    RegistroIPC reg;
    char buffer[500];
    // encabezado
    fgets(buffer, sizeof(buffer), fPArchIpc);

    //Buscamos los IPCs
    while (fgets(buffer, sizeof(buffer), fPArchIpc) && (ipcInicio == 0 || ipcFin == 0)) {
        if (trozarLineaDivisiones(buffer, &reg) && strcmpi(reg.descripcion, "nivel general") == 0 && strcmpi(reg.region, region) == 0) {
            int fechReg = convertirFechaStringAInt(reg.periodo);

            if (fechaInicio == fechReg)
                ipcInicio = atof(reg.indice_ipc);
            if (fechaFin == fechReg)
                ipcFin = atof(reg.indice_ipc);
        }
    }
    fclose(fPArchIpc);

    //Informamos los resultados
    if (ipcInicio == 0 || ipcFin == 0) {
        printf("No se encontraron los datos del IPC");
    }else {
        printf("Monto inicial: %.2f \n Monto ajustado: %.2f\n", monto, monto * (ipcFin / ipcInicio));
        printf("Variacion porcentual: %.2f\n", (ipcFin / ipcInicio - 1) * 100);
    }

}
int convertirFechaStringAInt(const char *fechaFormateada) {
    char mesNombre[30];
    int anioNum;
    int mesNum;

    if (sscanf(fechaFormateada, "%s - %d", mesNombre, &anioNum) != 2) {
        return 0; // Error de formato
    }
    mesNum = obtenerNumeroMes(mesNombre);
    if (mesNum > 0) {
        return (anioNum * 100) + mesNum;
    }
    return 0;
}
int obtenerNumeroMes(const char *mesTexto) {
    const char *meses[] = {
        "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };

    const char **ptr = meses;
    int numero = 1;

    while (numero <= 12) {
        if (strcmpi(mesTexto, *ptr) == 0)
            return numero;
        ptr++;
        numero++;
    }
    return 0;
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
void calcularIPCPorGrupos(const char* nomArchivo_ipc, Vector* grupos) {
    FILE* fPArchIpc = fopen(nomArchivo_ipc,"r");
    if(!fPArchIpc)
    {
        puts("ERROR PARA ABRIR ARCHIVO DE DIVISIONES");
        return;
    }
    char buffer[500];
    fgets(buffer, sizeof(buffer), fPArchIpc);
    RegistroIPC reg;
    Clasificacion clasificacion;
    while (fgets(buffer, sizeof(buffer), fPArchIpc)) {
        if (trozarLineaDivisiones(buffer, &reg)) {
            if (strcmpi(reg.region, "nacional") == 0) {
                clasificacion.periodo = convertirFechaStringAInt(reg.periodo);
                strcpy(clasificacion.descripcion, reg.descripcion);
                clasificacion.indice_ipc = atof(reg.indice_ipc);
                strcpy(clasificacion.region, reg.region);
                clasificarGrupo(reg.descripcion, clasificacion.grupo);
                vectorInsertar(grupos, &clasificacion);
            }
        }
    }
    mostrarPromedios(grupos);

    fclose(fPArchIpc);
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
/*
// 7
    // Función para convertir AAAAMM → AAAA-MM-DD
void convertirPeriodo(const char *periodo, char *fechaConvertida)
{
    char anio[5];
    char mes[3];

    strncpy(anio, periodo, 4);
    anio[4] = '\0';
    strncpy(mes, periodo + 4, 2);
    mes[2] = '\0';

    sprintf(fechaConvertida, "%s-%s-01", anio, mes);
}

int septimoEjercicio(const char *nombreDeEntrada)
{
    FILE *entrada = fopen(nombreDeEntrada, "r");
    FILE *salida = fopen("serie_ipc_aperturas_convertido.csv", "w");

    if (!entrada || !salida)
    {
        perror("Error al abrir archivos");
        return 1;
    }

    char linea[MAX_LINEA];
    int esPrimera = 1;

    while (fgets(linea, sizeof(linea), entrada))
    {
        // Si es la primera línea (encabezado), agregamos una nueva columna
        if (esPrimera)
        {
            fprintf(salida, "%s,Fecha_convertida\n", strtok(linea, "\n"));
            esPrimera = 0;
            continue;
        }

        // Variables para almacenar campos
        char codigo[21], descripcion[31], clasificador[31];
        char periodo[7], indice[32], v_m_ipc[32], v_i_a_ipc[32], region[10];
        char fecha[11];

        // Leemos línea separada por punto y coma
        int campos = sscanf(linea, "%20[^;];%30[^;];%30[^;];%6[^;];%31[^;];%31[^;];%31[^;];%9[^\n]",
                            codigo, descripcion, clasificador, periodo,
                            indice, v_m_ipc, v_i_a_ipc, region);

        if (campos == 8)
        {
            convertirPeriodo(periodo, fecha);
            fprintf(salida, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                    codigo, descripcion, clasificador, periodo,
                    indice, v_m_ipc, v_i_a_ipc, region, fecha);
        }
    }

    fclose(entrada);
    fclose(salida);
   remove(nombreDeEntrada);
    rename("serie_ipc_aperturas_convertido.csv", nombreDeEntrada);
    return 0;
}
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
        printf("\nError al abrir el archivo serie_ipc_aperturas.csv");
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
    double variacionTotal = ((ipcMax / ipcInicio)-1) * 100;

    printf("\n--- Calculadora de Alquileres ---\n");
    printf("Monto inicial:       $ %.2f\n", monto);
    printf("Monto ajustado:      $ %.2f\n", montoAjustadoTotal);
    printf("Variacion porcentual:  %.2f %%\n", variacionTotal);

    FILE* archivo_binario = fopen(ARCHIVO_BINARIO_SALIDA, "wb");
    if (archivo_binario == NULL) {
        perror("Error al crear archivo binario de salida");
        return;
    }

    
    //printf("\n--- Detalle Mes a Mes ---\n");
    //printf("-----------------------------------------------------------\n");
    //printf("%-10s %-12s %-12s %-15s\n", "Periodo", "Indice", "Variacion %", "Monto ajustado");
    //printf("-----------------------------------------------------------\n");
    
    FilaTablaAlquiler fila;

    //Iterar sobre los registros guardados para generar la tabla
    for (int i = 0; i < count; i++) {
        ra = registros_encontrados[i];

        // Llenar la struct 'fila' para la tabla
        sprintf(fila.periodo, "%d-%02d", ra.periodo / 100, ra.periodo % 100); // anio y mes
        fila.indice = ra.indice_ipc;
        //variación y monto contra el índice INICIAL
        fila.variacionPct = ((ra.indice_ipc / ipcInicio )-1) * 100;
        fila.montoAjustado = monto * (ra.indice_ipc / ipcInicio);

        
        //printf("%-10s %-12.2f %-12.2f %-15.2f\n",
        //       fila.periodo,
        //       fila.indice,
        //       fila.variacionPct,
        //       fila.montoAjustado);

        fwrite(&fila, sizeof(FilaTablaAlquiler), 1, archivo_binario);
    }

    fclose(archivo_binario);
    leerMostrarTablaBinario(ARCHIVO_BINARIO_SALIDA);
}
void leerMostrarTablaBinario(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        printf("\nError al abrir el archivo binario '%s' para lectura.\n", nombreArchivo);
        return;
    }
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
*/
