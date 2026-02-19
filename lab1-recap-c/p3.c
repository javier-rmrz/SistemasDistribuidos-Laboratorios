#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    long int min, max;
    for (int i=1; i<argc; i++) {
        char *end;
        long val = strtol(argv[i], &end, 10);

        if (*end != '\0') {
            printf("El argumento %i: Error de conversión.\n", i);
        }
        else {
            if (i == 1) {
                min = val;
                max = val;
            }
            else if (val < min) {
                min = val;
            }
            else if (val > max) {
                max = val;
            }
        }
    }
    printf("Valor mínimo = %ld\n", min);
    printf("Valor máximo = %ld\n", max);
}