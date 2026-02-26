#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

// Este programa copia un archivo pasado como parámetro en otro. El productor lee del fichero e inserta
// los caracteres en un buffer, y el consumidor extrae los caracteres del buffer y los escribe en el fichero de
// salida. Compile, ejecute y analice el funcionamiento del programa. Modifique el programa para que la ejecución 
// de los dos procesos sea correcta. Tenga en cuenta que cuando el proceso productor finaliza la lectura del fichero 
// debe notificar al consumidor dicho evento. Utilice en la solución mutex y variables condicionales.


#define BUF_SIZE	256

char buffer[BUF_SIZE];
int  n_elementos = 0;
int in_pos = 0;
int out_pos = 0;
int finished = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

void *productor(void *arg) {
	int *f = (int *)arg;
	char c;
	ssize_t n;

	while ((n = read(*f, &c, 1)) > 0) {
		pthread_mutex_lock(&mtx);
		while (n_elementos == BUF_SIZE) {
			pthread_cond_wait(&not_full, &mtx);
		}

		buffer[in_pos] = c;
		in_pos = (in_pos + 1) % BUF_SIZE;
		n_elementos++;

		pthread_cond_signal(&not_empty);
		pthread_mutex_unlock(&mtx);
	}

	if (n < 0) {
		perror("read");
	}

	pthread_mutex_lock(&mtx);
	finished = 1;
	pthread_cond_broadcast(&not_empty);
	pthread_mutex_unlock(&mtx);

	return NULL;
}

void *consumidor(void *arg){
	int *f = (int *)arg;
	char c;

	for (;;) {
		pthread_mutex_lock(&mtx);
		while (n_elementos == 0 && !finished) {
			pthread_cond_wait(&not_empty, &mtx);
		}

		if (n_elementos == 0 && finished) {
			pthread_mutex_unlock(&mtx);
			break;
		}

		c = buffer[out_pos];
		out_pos = (out_pos + 1) % BUF_SIZE;
		n_elementos--;

		pthread_cond_signal(&not_full);
		pthread_mutex_unlock(&mtx);

		if (write(*f, &c, 1) < 0) {
			perror("write");
			exit(0);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_attr_t attr;
	pthread_t thid[2];
	int fe, fs;


	if (argc < 3) {
		printf("Uso: %s  <fichero entrada> <fichero salida> \n", argv[0]);
		exit(0);
	}

	fe = open (argv[1], O_RDONLY);
	if (fe < 0) {
		printf("Error al abrir %s\n", argv[1]);
		exit(0);
	}

	fs = creat (argv[2], 0700);
	if (fs < 0) {
		printf("Error al crear %s\n", argv[2]);
		close(fe);
		exit(0);
	}

	pthread_attr_init(&attr);

	if (pthread_create(&thid[0], NULL, productor, &fe) != 0){
		printf("Error al crear el productor\n");
		exit(0);
	}

	if (pthread_create(&thid[1], NULL, consumidor, &fs) != 0){
		printf("Error al crear el consumidor\n");
		exit(0);
	}

	pthread_join(thid[0], NULL);
	pthread_join(thid[1], NULL);

	close(fe);
	close(fs);

	exit(0);

}
	
// PROBLEMAS EN VERSIÓN ORIGINAL:
// - No había mutex en el acceso al búffer, por lo que aparecen condiciones de carrera.
// - El consumidor no para, seguirá de forma indefinida al no haber un final del búffer indicado por el productor.
// - No se actualiza n_elementos, por lo que no se controlaba un lleno o vacío del búffer.
	
// MODIFICACIONES REALIZADAS:
// - Adición de un buffer real circular (in_pos, out_pos, n_elementos).
// - Sincronización completa con las variables condicionales y los mutex.
// - Notificación de fin con 'finished'.
// - Firmas correctas de hilos para evitar el cast.