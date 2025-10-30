#include "funciones.h"
int main() {
    int err=TODO_OK;
    
    //EJ 1,2,3,4
    err = actualizarArchivoDivisiones("serie_ipc_divisiones.csv","arch_temporal.csv");
    if(err == TODO_OK)
    {
        printf("\nARCHIVO DE DIVISIONES ACTUALIZADO CORRECTAMENTE\n");
        //EJ5
        printf("\nCALCULAR AJUSTE POR INFLACION\n");
        calcularMontoAjustadoPorIPC("serie_ipc_divisiones.csv");
    }
    //EJ 6
    Vector vector;
    err = vectorCrear(&vector, sizeof(Clasificacion));
    if(err == TODO_OK)
    {
        printf("\nINDICE IPC POR GRUPOS (BIENES | SERVICIOS)\n");
        calcularIPCPorGrupos("serie_ipc_divisiones.csv", &vector);
        vectorDestruir(&vector);
    }
    //EJ 7,8
    err = actualizarArchivoAperturas("serie_ipc_aperturas.csv","arch_temporal.csv");
        
    //EJ 9
    if(err == TODO_OK)
    {
        printf("\nCALCULAR AJUSTE ALQUILER\n");
        calcularAjusteAlquiler("serie_ipc_aperturas.csv");
    }
    return 0;
}

