#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS	2
#define ITER 		10

// Modifique el programa anterior de forma que cada uno de los threads imprima su identificador (0
// o 1) de forma correcta. Utilice para ello mutex y variables condicionales.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turno = 0;

void *funcion(void *arg) { // Solución del problema 2 del p1.c
    int mid = *(int *)arg;
	int j;
	double k;
	int s;

    pthread_mutex_lock(&mutex);
    while (turno != mid) {
        pthread_cond_wait(&cond, &mutex);
    }

    printf("El identificador de este thread es %d\n", mid);

    turno = (turno + 1) % NUM_THREADS;  // le toca al siguiente
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) (k * 100000));
		printf("Ejecuta el thread %d iteracion %d \n", mid, j );
	}

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int j;
    pthread_attr_t attr;
    pthread_t thid[NUM_THREADS];
    int ids[NUM_THREADS]; // Solución del problema 1 del p1.c

    pthread_attr_init(&attr);

    for (j = 0; j < NUM_THREADS; j++) {
        ids[j] = j; // Solución del problema 1 del p1.c
        if (pthread_create(&thid[j], &attr, funcion, &ids[j]) != 0) { // Solución del problemas 1 y 2 del p1.c
            perror("pthread_create");
            exit(1);
        }
    }

    for (j = 0; j < NUM_THREADS; j++)
        pthread_join(thid[j], NULL);

    return 0;
}
	



