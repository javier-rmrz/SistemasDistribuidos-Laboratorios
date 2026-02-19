#include <stdio.h>
#include <stdlib.h>

//Ordenar de menor a mayor los elementos del array con qsort()
int comparar_enteros(const void *a, const void *b) {
    int x = * (const int *)a;
    int y = * (const int *)b;

    if (x > y) {return 1;}
    else if (x < y) {return -1;}
    return 0;
}


int main(int argc, char *argv[]) {
    int *argumentos = malloc((argc - 1) * sizeof(int *));
    for (int i = 1; i < argc; i++) {
        char *end;
        argumentos[i-1] = (int) strtol(argv[i], &end, 10);
    }

    qsort(argumentos, argc - 1, sizeof(int), comparar_enteros);

    for (int i=0; i < argc - 1; i++) {
        printf("%d\n", argumentos[i]);
    }
}