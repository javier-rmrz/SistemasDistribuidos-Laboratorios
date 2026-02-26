#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS	10
#define ITER 		10

// Modifique el programa de p4.c de forma que ahora haya 10 hilos que ejecuten de forma alternada el código
// Con el código de la versión anterior, basta con modificar el NUM_THREADS a 10, ya que el funcionamiento es igual.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turno = 0;

void *funcion(void *arg) { // Solución del problema 2 del p1.c
    int mid = *(int *)arg;
	int j;
	double k;
	unsigned int s = (unsigned int)time(NULL) ^ (unsigned int)mid;

	for (j = 0; j < ITER; j++) {
		pthread_mutex_lock(&mutex);
		while (turno != mid) {
			pthread_cond_wait(&cond, &mutex);
		}

		k = (double) rand_r(&s) / RAND_MAX;
		usleep((int) (k * 100000));
		printf("Ejecuta el thread %d iteracion %d \n", mid, j);

		turno = (turno + 1) % NUM_THREADS;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);
	}

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int j;
    pthread_attr_t attr;
    pthread_t thid[NUM_THREADS];
    int ids[NUM_THREADS];

    pthread_attr_init(&attr);

    for (j = 0; j < NUM_THREADS; j++) {
        ids[j] = j;
        if (pthread_create(&thid[j], &attr, funcion, &ids[j]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (j = 0; j < NUM_THREADS; j++)
        pthread_join(thid[j], NULL);

    return 0;
}
	



