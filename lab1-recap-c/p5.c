#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "list.c"

void ImprimirLista (List lista) {
    while (lista != NULL) {
        printf("Key=%s, Value=%d\n", lista->key, lista->value);
        lista = lista->next;
    }
}

int main(int argc, char *argv[]) {

    //Definición de lista enlazada
    List lista;
    int err = 0;
    char key[MAX_KEY_LENGTH];

    err = init(&lista);
    if (err == -1) {
        printf("Error in init\n");
        return -1;
    }

    for (int i=1; i<argc; i++) {
        char *end;
        int x = strtol(argv[i], &end, 10);
        sprintf(key, "%d", i);
        set(&lista, key, x);
    }
    ImprimirLista(lista);
}
