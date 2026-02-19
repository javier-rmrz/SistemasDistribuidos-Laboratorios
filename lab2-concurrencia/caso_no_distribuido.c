#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int a_neltos = 0;
int *a_values[100];
char *a_keys[100];

int N = 10;
char *A = "nombre";
int E = 1;
int V = 0x123;

// Busca el índice del array de nombre dado dentro del array de arrays
int buscar(char* nombre){
    int index = -1;

    for (int i = 0; i < a_neltos; i++){
        if (strcmp(a_keys[i], nombre) == 0){
            index = i;
            break;
        }
    }
    return index;
}

// Inserta un nuevo array de valores en el array de arrays
int insertar(char* nombre, int n){
    a_values[a_neltos] = malloc(n * sizeof(int));
    if (a_values[a_neltos] == NULL) {
        return -1;
    }

    a_keys[a_neltos] = strdup(nombre);
    if (a_keys[a_neltos] == NULL) {
        free(a_values[a_neltos]);
        return -1;
    }

    a_neltos++;
    return 1;
}

// FUNCIÓN INIT: Inicializa el array de arrays y el array de keys
//  Devuelve 1 si se crea un nuevo array al final del array de arrays correspondientes
//  Devuelve -1 si hay algún error de creación
//  Devuelve 0 si el array ya se ha creado (existe otro con el mismo nombre ya creado)
int init(char *nombre, int N){
    int index = buscar(nombre);
    if (index != -1){
        return 0;
    }

    index = insertar(nombre, N);
    if (index == -1){
        return -1;
    }

    return 1;
}

// FUNCIÓN SET: Posiciona un entero 'valor' en la posición 'i' del array 'nombre'
int set(char *nombre, int i, int valor) {
    int index = buscar(nombre);
    if (index == -1) {
        return -1;
    }
    a_values[index][i] = valor;
    return 1;
}

// FUNCIÓN GET: Recupera el valor del elemento 'i' del array 'nombre' y lo coloca en la posición de memoria 'valor'
int get(char *nombre, int i, int *valor) {
    int index = buscar(nombre);
    if (index == -1) {
        return -1;
    }
    *valor = a_values[index][i];
    return 1;
}

int main(){
    int ret, val;

    ret = init(A,N);
    if (ret<0) {printf("init: error code %d\n", ret); exit(-1); }
    ret = set (A, E, V) ;
    if (ret < 0) { printf("set: error code %d\n", ret); exit(-1); }
    ret = get (A, E, &val) ;
    if (ret < 0) { printf("get: error code %d\n", ret); exit(-1); }
    return 0;
}