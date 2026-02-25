#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS	2
#define ITER 		10

// Identificar problemas de ejecución que se pueden presentar al ejecutar el programa.

// - Problema 1: Se pasa la dirección de j a cada uno de los hilos, pero cuando el hilo despierta, el valor de j puede haberse incrementado ya. (ver img p1-1.png)
//               Puede solucionarse haciendo 
// 					ids[j] = j
// 					if (pthread_create(&thid[j], NULL, (void *) funcion, &id[j]) == -1){

// - Problema 2: La firma del hilos no es estándar, por lo que en ciertos entornos podría tener un comportamiento indefinido
//				 En definición, void (*)
//				 pthread_create espera una de tipo void *(*)(void *)
//				 Por tanto, se fuerza un cast automático que en este entorno funciona, pero en otros podría no hacerlo

// - Problema 3: En la función, s se declara pero no se inicializa, por lo que no se puede usar rand_r ya que requiere de un valor específico que tenga asignado.
// 				 En este caso, se pasa una dirección de variable, cuyo valor no se tiene controlado.
//				 Solución: Asignar un valor para la variable antes del bucle.
//				 unsigned int s = (unsigned int)time(NULL) ^ (unsigned int)mid;
//				 for (j = 0; j < ITER; j++) {
//			         k = (double)rand_r(&s) / RAND_MAX;

void funcion(int *id) {
	int j;
	int s;
	double k;
	int mid = *id;  // cada thread recibe un número (0 o 1)

	for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) (k * 100000)); // duerme entre 0 y 100 ms
		printf("Ejecuta el thread %d iteracion %d \n", mid, j );
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

	for (j = 0; j < NUM_THREADS; j++)
		if (pthread_create(&thid[j], NULL, (void *) funcion, &j) == -1){
			printf("Error al crear los threads\n");
			exit(0);
		}

	for (j = 0; j < NUM_THREADS; j++)
		pthread_join(thid[j], NULL);

	exit(0);

}
	



