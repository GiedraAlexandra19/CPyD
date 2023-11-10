#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

int counter = 0;
int thread_count = NUM_THREADS;
pthread_mutex_t barrier_mutex = PTHREAD_MUTEX_INITIALIZER; //Un mutex para garantizar la exclusión mutua al acceder a la barrera.
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;   //Una condición que se utiliza para que los hilos esperen en la barrera.

void* ThreadWork(void* arg) {
    // Simulación de trabajo
    printf("El hilo %ld esta funcionando....\n", (long)arg);

    // Barrera
    pthread_mutex_lock(&barrier_mutex);
    counter++;
    if (counter < thread_count) {
        // Espera a que los demás hilos alcancen la barrera
        pthread_cond_wait(&barrier_cond, &barrier_mutex);
    } else {
        // Último hilo que alcanza la barrera
        counter = 0;
        // Despierta a todos los hilos
        pthread_cond_broadcast(&barrier_cond);
    }
    pthread_mutex_unlock(&barrier_mutex);

    printf("El hilo %ld paso la barrera y continua su trabajo.\n", (long)arg);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Crear hilos
    for (long i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, ThreadWork, (void*)i);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Todos los hilos han completado su trabajo.\n");

    return 0;
}