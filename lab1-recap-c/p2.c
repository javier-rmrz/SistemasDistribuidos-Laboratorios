#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    for (int i=1; i<argc; i++) {
        char *end;
        long val = strtol(argv[i], &end, 10);
        if (*end != '\0') {
            printf("Argumento %i: Error de conversión.\n", i);
        }
        else {
            printf("Argumento %i: %ld\n", i, val);
        }
    }
}