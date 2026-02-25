#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS	2
#define ITER 		10


void funcion(int *id) {
	int j;
	int s;
	double k;
	int mid = *id;

	printf("El identificador de este thread es %d\n", mid); // Línea añadida para la impresión del id del thread

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) (k * 100000));
		// printf("Ejecuta el thread %d iteracion %d \n", mid, j );
	}
	
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int j;
	pthread_attr_t attr;
	pthread_t thid[NUM_THREADS];
	struct timeval t;
	int id[NUM_THREADS];  // Solución del problema de los ids

	gettimeofday(&t, NULL);
	srand(t.tv_sec);

	pthread_attr_init(&attr);

	for (j = 0; j < NUM_THREADS; j++) {
		id[j] = j;  // Solución del problema de los ids
		if (pthread_create(&thid[j], NULL, (void *) funcion, &id[j]) == -1){ // Solución del problema de los ids
			printf("Error al crear los threads\n");
			exit(0);
		}
	}
	for (j = 0; j < NUM_THREADS; j++)
		pthread_join(thid[j], NULL);

	exit(0);

}
	



