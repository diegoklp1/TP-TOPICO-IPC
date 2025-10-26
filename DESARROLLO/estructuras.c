#include <stdio.h>

#define FILENAME_APERTURAS "serie_ipc_aperturas.csv"
#define FILTRO_DESCRIPCION_ALQUILER "Alquiler de la vivienda"
#define ARCHIVO_BINARIO_SALIDA "tabla_alquileres.dat"
#define MAX_MESES_REPORTE 500

typedef struct {
    char codigo[32];
    char descripcion[128];
    char clasificador[64];
    char indice_ipc[21];
    double variacion_mensual;
    double variacion_interanual;
    char region[64];
    char periodo[21];   // AAAAMM
} RegistroIPC;

typedef struct {
    char fecha[30];
    char descripcion[128];
    int periodo;   // AAAAMM
    double indice_ipc;
    char region[64];
    char grupo[32];
} Clasificacion;

typedef struct {
    void* vector;
    size_t tamanioElemento;
    size_t cantidadElementos;
    size_t capacidad;
} Vector;

typedef struct {
    char periodo[8]; // Formato "YYYY-MM"
    double indice;
    double variacionPct;
    double montoAjustado;
} FilaTablaAlquiler;
