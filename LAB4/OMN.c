#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Definición de un nodo de la lista
typedef struct Node {
    int data;
    struct Node* next;
    pthread_mutex_t nodeMutex; //mutex específico para el nodo
} Node;

// Definición de la lista enlazada
typedef struct {
    Node* head;
} LinkedList;

// Inicialización de la lista
void initList(LinkedList* list) {
    list->head = NULL;
}

// Inserción de un nuevo nodo al principio de la lista
/*crea un nuevo nodo, inicializa su mutex, adquiere el mutex, 
inserta el nodo al principio de la lista y luego libera el mutex.*/
void insertNode(LinkedList* list, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    pthread_mutex_init(&newNode->nodeMutex, NULL);

    // Adquirir el mutex del nuevo nodo
    pthread_mutex_lock(&newNode->nodeMutex);

    // Insertar el nodo en la lista
    newNode->next = list->head;
    list->head = newNode;

    // Liberar el mutex del nuevo nodo
    pthread_mutex_unlock(&newNode->nodeMutex);
}

// Imprimir la lista
void printList(LinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        // Adquirir el mutex del nodo actual
        pthread_mutex_lock(&current->nodeMutex);

        printf("%d ", current->data);

        // Liberar el mutex del nodo actual
        pthread_mutex_unlock(&current->nodeMutex);

        current = current->next;
    }
    printf("\n");
}

// Función para un hilo que inserta elementos en la lista
void* insertThread(void* arg) {
    LinkedList* list = (LinkedList*)arg;

    for (int i = 0; i < 15; ++i) {
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
    LinkedList myList;
    initList(&myList);

    // Crear hilos para realizar operaciones en la lista
    pthread_t insertThreadID, printThreadID;
    pthread_create(&insertThreadID, NULL, insertThread, (void*)&myList);
    pthread_create(&printThreadID, NULL, printThread, (void*)&myList);

    // Esperar a que ambos hilos terminen
    pthread_join(insertThreadID, NULL);
    pthread_join(printThreadID, NULL);

    // Liberar recursos (destruir mutex de cada nodo)
    Node* current = myList.head;
    while (current != NULL) {
        pthread_mutex_destroy(&current->nodeMutex);
        Node* next = current->next;
        free(current);
        current = next;
    }

    return 0;
}