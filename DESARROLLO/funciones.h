#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "estructuras.c"
#include <stdbool.h>

// Convención nombre de funciones : primer palabra en minuscula, la siguiente empiezan con mayuscula.

void limpiarCampo(char *campo);
void mostrarPalabra(const char *p);
bool leerArchivoCompletoIPC(char* nomArch);
bool trozarLineaIPC(char buffer[], RegistroIPC *registro);

//1
char decodificarDigito(char c);
int decodificarFecha(int fechaE);
//2
void convertirFecha(int fecha, char* fechaTexto);
//3
void normalizarDescripcion(char *p);
//4
void convertirComaAPunto(char *p);
//5
void calcularMontoAjustadoPorIPC(FILE *archivo_ipc);
void buscarIPCs(FILE *archivo_ipc, char region[10], int fechaInicio, int fechaFin, double *ipcInicio, double *ipcFin, RegistroIPC ra);
void solicitarMonto(float *monto);
void solicitarRegion(char *region);
void solicitarPeriodos(int *fecha_inicio, int *fecha_fin);

#endif // FUNCIONES_H_INCLUDED
