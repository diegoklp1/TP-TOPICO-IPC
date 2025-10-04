#include <stdio.h>


typedef struct {
    char codigo[21];
    char descripcion[31];    
    char clasificador[31];   
    double indice_ipc;         
    double variacion_mensual;  
    double variacion_interanual; 
    char region[10];           
    int periodo;           // formato AAAAMM, 6 dígitos
} RegistroIPC;
