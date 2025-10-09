#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "estructuras.c"

// Convención nombre de funciones : primer palabra en minuscula, la siguiente empiezan con mayuscula.
void limpiarCampo(char *campo);
char decodificarDigito(char c);
int decodificarFecha(int fechaE);

void mostrarPalabra(const char *p);
void normalizarDescripcion(char *p);
void convertirFecha(int fecha, char* fechaTexto);
void convertirComaAPunto(char *p);

void calcularMontoAjustadoPorIPC(FILE *archivo_ipc);
void buscarIPCs(FILE *archivo_ipc, char region[10], int fechaInicio, int fechaFin, double *ipcInicio, double *ipcFin, RegistroIPC ra);
void solicitarMonto(float *monto);
void solicitarRegion(char *region);
void solicitarPeriodos(int *fecha_inicio, int *fecha_fin);

#endif // FUNCIONES_H_INCLUDED
