#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 3

int counter = 0;
int thread_count = NUM_THREADS;
sem_t count_sem;
sem_t barrier_sem;

void* ThreadWork(void* arg) {
    // Simulación de trabajo
    printf("El hilo %ld esta funcionando...\n", (long)arg);

    // Barrera
    /*Si un hilo es el último en llegar a la barrera, 
	reinicia counter y libera a los demás hilos. De lo contrario, 
	incrementa counter y espera a que la barrera se libere.*/
    sem_wait(&count_sem);
    if (counter == thread_count - 1) {
        counter = 0;
        sem_post(&count_sem);
        for (int j = 0; j < thread_count - 1; j++) {
            sem_post(&barrier_sem);
        }
    } else {
        counter++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }

    printf("El hilo %ld paso la barrera y continua su trabajo.\n", (long)arg);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Inicialización de semáforos
    sem_init(&count_sem, 0, 1);    // count_sem se inicializa a 1
    sem_init(&barrier_sem, 0, 0);  // barrier_sem se inicializa a 0

    // Crear hilos
    for (long i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, ThreadWork, (void*)i);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Destruir los semáforos
    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);

    printf("Todos los hilos han completado su trabajo.\n");

    return 0;
}