#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>


#define N 1E7
#define d 1E-7

#define SIZE 20

double PI;

typedef struct {
    int id;
    double partial;
} thread_data_t;
// Para poder evitar las carreras, se crea una estructura que almacenará el resultado parcial de cada hilo.
// Antes, se competía por PI para sumar y no había un resultado determinista. Ahora la suma la hace el main.


void *calcula(void *arg){
    thread_data_t *data = (thread_data_t *)arg;
    int id = data->id;
    double result = 0.0, x;
    int i;

    for (i = id; i < N; i = i + SIZE) {
        x = d * i;
        result += sqrt(4 * (1 - x * x));
    }

    data->partial = d * 2 * result;

    return NULL;
}

int main (int argc, char* argv[])
{
    int i;
    struct timeval t1, t2;
    long t;

    pthread_t th[SIZE];
    thread_data_t td[SIZE];

    gettimeofday(&t1, 0);

    for (i = 0; i < SIZE; i++) {
        td[i].id = i;
        td[i].partial = 0.0;
        if (pthread_create(&th[i], NULL, calcula, &td[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    PI = 0.0;
    for (i = 0; i < SIZE; i++) {
        pthread_join(th[i], NULL);
        PI += td[i].partial;
    }

    gettimeofday(&t2, 0);
    t = (t2.tv_sec-t1.tv_sec)*1000000 + t2.tv_usec-t1.tv_usec;
    printf("El tiempo es %f ms\n", (t / 1000.0));

    printf("PI = %.9f\n", PI);

    return 0;
}