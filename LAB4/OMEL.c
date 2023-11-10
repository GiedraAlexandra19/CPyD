#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Definición de un nodo de la lista
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Definición de la lista enlazada con un solo mutex
typedef struct {
    Node* head;
    pthread_mutex_t listLock;
} LinkedList;

// Inicialización de la lista
void initList(LinkedList* list) {
    list->head = NULL;
    pthread_mutex_init(&list->listLock, NULL);
}

// Inserción de un nuevo nodo al principio de la lista
void insertNode(LinkedList* list, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;

    pthread_mutex_lock(&list->listLock);

    newNode->next = list->head;
    list->head = newNode;

    pthread_mutex_unlock(&list->listLock);
}

// Imprimir la lista
void printList(LinkedList* list) {
    pthread_mutex_lock(&list->listLock);

    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");

    pthread_mutex_unlock(&list->listLock);
}

// Función para un hilo que inserta elementos en la lista
void* insertThread(void* arg) {
    LinkedList* list = (LinkedList*)arg;

    for (int i = 0; i < 10; ++i) {
        insertNode(list, i);
    }

    return NULL;
}

// Función para un hilo que imprime la lista
void* printThread(void* arg) {
    LinkedList* list = (LinkedList*)arg;

    // Espera corta para asegurar que haya elementos en la lista
    usleep(100);

    printList(list);

    return NULL;
}

int main() {
    // Inicialización de la lista
    LinkedList myList;
    initList(&myList);

    // Creación de hilos
    pthread_t insertThreadID, printThreadID;
    pthread_create(&insertThreadID, NULL, insertThread, (void*)&myList);
    pthread_create(&printThreadID, NULL, printThread, (void*)&myList);

    // Esperar a que ambos hilos terminen
    pthread_join(insertThreadID, NULL);
    pthread_join(printThreadID, NULL);

    // Destrucción del mutex de la lista
    pthread_mutex_destroy(&myList.listLock);

    return 0;
}