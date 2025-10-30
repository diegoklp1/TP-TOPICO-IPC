#include "funciones.h"
int main() {
    int err=TODO_OK;
    
    //EJ 1,2,3,4
    
    err = actualizarArchivoDivisiones("serie_ipc_divisiones.csv","arch_temporal.csv");
    if(err == TODO_OK)
    {
        //EJ 5 (Se puede probar monto=15000, region=GBA, fechas : 201701 - 201712)
        printf("\nCALCULAR AJUSTE POR INFLACION\n");
        //calcularMontoAjustadoPorIPC("serie_ipc_divisiones.csv");
    }
    //EJ 6
    Vector vector;
    err = vectorCrear(&vector, sizeof(Clasificacion));
    if(err == TODO_OK)
    {
        calcularIPCPorGrupos("serie_ipc_divisiones.csv", &vector);
        vectorDestruir(&vector);
    }
    //EJ 7,8
    err = actualizarArchivoAperturas("serie_ipc_aperturas.csv","arch_temporal.csv");
        
    //EJ 9 (1500 , 2 , 201701)
    if(err == TODO_OK)
    {
        printf("\nCALCULAR AJUSTE ALQUILER\n");
        calcularAjusteAlquiler("serie_ipc_aperturas.csv");
    }
    return 0;
}

