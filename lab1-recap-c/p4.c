#include <stdio.h>
#include <stdlib.h>

void ImprimirArray (int argc, char *argv[]) {
    for (int i=1; i<argc; i++) {
        printf("Argumento %i: %s\n", i, argv[i]);
    }
}

int main(int argc, char *argv[]) {

    //Definición de lista dinámica
    char * lista = NULL;
    lista = (char *) malloc(sizeof(char) * argc);
    ImprimirArray(argc, argv);
}
