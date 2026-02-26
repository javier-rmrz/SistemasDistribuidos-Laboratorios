#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_LECTORES	10
#define NUM_THREADS	11    // 10 lectores y 1 escritor
#define ITER 		40

pthread_mutex_t mtx;
pthread_cond_t can_read;
pthread_cond_t can_write;

int active_readers = 0;
int active_writer = 0;   // 0/1
int		recurso = 0;

void *lector(void *arg) {
	int j;
	unsigned int s = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self();
	double k;

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) 1+(k * 6000000)); // duerme entre 1 y 9 s

		if (pthread_mutex_lock(&mtx) != 0)
			printf(".......  Error en mutex lock (lector) \n");
		while (active_writer) {
			pthread_cond_wait(&can_read, &mtx);
		}
		active_readers++;
		if (pthread_mutex_unlock(&mtx) != 0)
			printf(".......  Error en mutex unlock (lector) \n");

		printf("       Ejecuta el lector %lu lee  %d \n", (unsigned long int) pthread_self(), recurso );

		if (pthread_mutex_lock(&mtx) != 0)
			printf(".......  Error en mutex lock (lector fin) \n");
		active_readers--;
		if (active_readers == 0) {
			pthread_cond_signal(&can_write);
		}
		if (pthread_mutex_unlock(&mtx) != 0)
			printf(".......  Error en mutex unlock (lector fin) \n");
	}
	
	pthread_exit(NULL);

}

void *escritor(void *arg) {
	int j;
	unsigned int s = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ 0x9e3779b9u;
	double k;

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) 2 + (k * 5000000)); // duerme entre 2 y 7 s

		pthread_mutex_lock(&mtx);
		while (active_writer || active_readers > 0) {
			pthread_cond_wait(&can_write, &mtx);
		}
		active_writer = 1;
		pthread_mutex_unlock(&mtx);

		recurso = recurso + 1;
		printf("                                                          Escritor: Nuevo valor %d\n", recurso);

		pthread_mutex_lock(&mtx);
		active_writer = 0;
		// Prioridad a lectores: despertar a todos los lectores primero
		pthread_cond_broadcast(&can_read);
		pthread_cond_signal(&can_write);
		pthread_mutex_unlock(&mtx);
	
	}
	
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int j;
	pthread_attr_t attr;
	pthread_t thid[NUM_THREADS];
	struct timeval t;

	gettimeofday(&t, NULL);
	srand(t.tv_sec);	// se inicializa la semilla de nª pseudoaleatorios

	pthread_attr_init(&attr);

	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&can_read, NULL);
	pthread_cond_init(&can_write, NULL);


	for (j = 0; j < NUM_LECTORES; j++)
		if (pthread_create(&thid[j], NULL, lector, NULL) != 0){
			printf("Error al crear los threads lectores\n");
			exit(0);
		}
	if (pthread_create(&thid[j], NULL, escritor, NULL) != 0){
		printf("Error al crear el escritor\n");
		exit(0);
	}

	for (j = 0; j < NUM_THREADS; j++)
		pthread_join(thid[j], NULL);

	exit(0);

}
	

// PROBLEMA: El primer lector bloquea el mutex, pero el último lector es quien lo desbloquea, lo cual no se puede con 
// 			 un mutex dado que debe desbloquearlo el mismo lector que lo bloqueó. Esto provoca que el mutex_recurso se
// 			 se quede bloqueado permanentemente, mientras que el escritor se queda atascado en él.
