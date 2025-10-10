#include <stdio.h>


typedef struct {
    char codigo[32];
    char descripcion[128];
    char clasificador[64];
    double indice_ipc;
    double variacion_mensual;
    double variacion_interanual;
    char region[64];
    int periodo;   // AAAAMM
    char fecha_convertida[30];
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