#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "estructuras.c"
#include <stdbool.h>

// Convención nombre de funciones : primer palabra en minuscula, la siguiente empiezan con mayuscula.

//void limpiarCampo(char *campo);
void mostrarPalabra(const char *p);
bool leerRegistroIPC(char* nomArch, RegistroIPC* reg);
bool leerArchivoCompletoIPC(char* nomArch);
bool trozarLineaDivisiones(char buffer[], RegistroIPC *registro);

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
void calcularMontoAjustadoPorIPC(FILE *archivo_ipc);


void solicitarMonto(float *monto);

void solicitarRegion(char *region);

void solicitarFecha(int *fecha_inicio);

//6
void calcularIPCPorGrupos(FILE *archivo_ipc, Vector* vector);
void mostrarPromedios(Vector* vec);
void clasificarGrupo(const char* descripcion, char *grupo);
//7
int septimoEjercicio(const char *nombreDeEntrada);

void convertirPeriodo(const char *periodo, char *fechaConvertida);

//9
void leerMostrarTablaBinario(const char*);


static bool redimensionarVector(Vector* vec, size_t cap);

bool vectorCrear(Vector* vec, size_t tamElem);
void vectorDestruir(Vector* vec);

int vectorInsertar(Vector* vec, void* elem);
#endif // FUNCIONES_H_INCLUDED

