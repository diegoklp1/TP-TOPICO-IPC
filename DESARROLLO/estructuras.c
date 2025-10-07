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

