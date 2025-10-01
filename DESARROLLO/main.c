#include <stdio.h>
#include <stdlib.h>


int main(void) {
    char periodoCod[] = "749681";
    char periodoDec[7];

    decodificarFecha(periodoCod, periodoDec);
    mostrarPalabra(periodoDec);

    return 0;
}

char decodificarDigito(char c) {
    switch (c) {
        case '7': return '0';
        case '4': return '1';
        case '9': return '2';
        case '8': return '3';
        case '0': return '4';
        case '6': return '5';
        case '1': return '6';
        case '3': return '7';
        case '2': return '8';
        default:  return '?';
    }
}

void decodificarFecha(const char *fechaE, char *fechaS ) {

    const char *i = fechaE;
    char *j = fechaS;

    while (*i != '\0') {
        *j++ = decodificarDigito(*i++);
    }
    *j = '\0';
}

void mostrarPalabra(const char *p) {
    while (*p)
        putchar(*p++);
    putchar('\n');
}