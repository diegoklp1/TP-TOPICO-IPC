#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "estructuras.c"

// Convención nombre de funciones : primer palabra en minuscula, la siguiente empiezan con mayuscula.

char decodificarDigito(char c);
int decodificarFecha(int fechaE);
void mostrarPalabra(const char *p);
void normalizarDescripcion(char *p);
void convertirFecha(int fecha, char* fechaTexto);


#endif // FUNCIONES_H_INCLUDED
