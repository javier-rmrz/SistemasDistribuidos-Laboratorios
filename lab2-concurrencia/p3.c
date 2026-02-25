#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS	2
#define ITER 		10

// Modifique el programa anterior de forma que el código siguiente de cada uno de los threads se
// ejecute en exclusión mutua. Utilice para ello un mutex. El nombre de este programa será p3.c. Para
// compilarlo utilice make p3. Esta solución se puede utilizar para acceder en exclusión mutua a algún tipo de
// recurso o fragmento de código que sea necesario proteger. En este caso no existe ningún problema dado
// que la ejecución de este fragmento de código no da lugar a posibles condiciones de carrera. Sin embargo, el
// objetivo es ver cómo resolver esta situación en una aplicación en la que sea necesario.

// Problemas
// - Contener trozos tan grandes de código en mutex implica un uso de recursos muy superior, y dentro de las funciones de este código hay muchas sentencia que no requieren
//   usar un mutex. Esto es de especial importancia en el bucle de 10 iteraciones.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *funcion(void *arg) {
    int mid = *(int *)arg;
    int j;
	int s;
	double k;

    pthread_mutex_lock(&mutex); // Uso del mutex

    printf("El identificador de este thread es %d\n", mid);
    for(j=0 ; j < ITER; j++) {
		k = (double) rand_r((unsigned int *) &s) / RAND_MAX;	
		usleep((int) (k * 100000));
		printf("Ejecuta el thread %d iteracion %d \n", mid, j );
	}

    pthread_mutex_unlock(&mutex);

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
	



