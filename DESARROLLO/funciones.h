#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "estructuras.c"
#include <stdbool.h>

// Convención nombre de funciones : primer palabra en minuscula, la siguiente empiezan con mayuscula.

void limpiarCampo(char *campo);
void mostrarPalabra(const char *p);
bool leerRegistroIPC(char* nomArch, RegistroIPC* reg);
bool leerArchivoCompletoIPC(char* nomArch);
bool trozarLineaDivisiones(char buffer[], RegistroIPC *registro);
bool trozarLineaAperturas(char linea[], RegistroIPC *registro);

int actualizarArchivoDivisiones(const char* nomArchDivisiones,const char* nomArchTemporal);
//1
char decodificarDigito(char c);
void decodificarFecha(char* fechaE);
//2
void convertirFecha(char* fecha);
//3
void normalizarDescripcion(char *p);
//4
void convertirComaAPunto(char *p);
//5
void calcularMontoAjustadoPorIPC(const char* nomArchivoIpc);

void solicitarMonto(float *monto);
void solicitarRegion(char *region);
void solicitarFecha(int *fecha_inicio);

int convertirFechaStringAInt(const char *fechaFormateada);
int obtenerNumeroMes(const char *mesTexto);
//6
void calcularIPCPorGrupos(const char* nomArchivo_ipc, Vector* grupos);
void mostrarPromedios(Vector* vec);
void clasificarGrupo(const char* descripcion, char *grupo);

int actualizarArchivoAperturas(const char* nomArchAper,const char* nomArchAperTemp);
//7
void convertirFormatoFecha(char *periodo);
//8 Funcion4
//9
void calcularAjusteAlquiler(const char* nomArchAperturas);
void leerMostrarTablaBinario(const char*);


static bool redimensionarVector(Vector* vec, double factorInc);
int vectorCrear(Vector* vector, size_t tamElem);
void vectorDestruir(Vector* vec);
int vectorInsertar(Vector* vec, void* elem);
#endif // FUNCIONES_H_INCLUDED

