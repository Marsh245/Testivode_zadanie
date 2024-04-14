#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// DoublyLinkedist node
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

// Function Dewclaring
void* countZeroBits(void* arg);
void* countOneBits(void* arg);
void insertAtEnd(Node** head, int data);
void printList(Node* head);


Node* head = NULL;
int zeroBitCount = 0;
int oneBitCount = 0;
int zeroBitNodesCount = 0;
int oneBitNodesCount = 0;

pthread_mutex_t zeroBitMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oneBitMutex = PTHREAD_MUTEX_INITIALIZER;

//thread arguments Struct
typedef struct {
    Node* node;
} ThreadArgs;

// Creating DoyubleLibnkedList fucntion
void createList(int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        insertAtEnd(&head, rand() % 10);
    }
}

// Function to insert a node at the end of the list
void insertAtEnd(Node** head, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (*head == NULL) {
        newNode->prev = NULL;
        *head = newNode;
        return;
    }

    Node* last = *head;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = newNode;
    newNode->prev = last;
}

// Function to count zero bits in node data including insignificant bits
void* countZeroBits(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    Node* node = args->node;

    int data = node->data;
    int countZero = 0;
    for (int i = 0; i < sizeof(int) * 8; i++) {
        if (!(data & (1 << i))) {
            countZero++;
        }
    }

    pthread_mutex_lock(&zeroBitMutex);
    zeroBitCount += countZero;
    zeroBitNodesCount++;
    pthread_mutex_unlock(&zeroBitMutex);

    pthread_exit(NULL);
}

// Function to count one bits in node data
void* countOneBits(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    Node* node = args->node;

    int data = node->data;
    int countOne = 0;
    for (int i = 0; i < sizeof(int) * 8; i++) {
        if (data & (1 << i)) {
            countOne++;
        }
    }

    pthread_mutex_lock(&oneBitMutex);
    oneBitCount += countOne;
    oneBitNodesCount++;
    pthread_mutex_unlock(&oneBitMutex);

    pthread_exit(NULL);
}

// Function to print the doubly linked list
void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    int size;
    printf("Enter the size of the list: ");
    scanf("%d", &size);

    createList(size);

    printf("List created with random numbers:\n");
    printList(head);

    pthread_t zeroBitThread, oneBitThread;

    Node* current = head;
    Node* tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }

    while (current != NULL && tail != NULL && current != tail && current->next != tail) {
        ThreadArgs* zeroBitArgs = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        if (zeroBitArgs == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        zeroBitArgs->node = current;

        if (pthread_create(&zeroBitThread, NULL, countZeroBits, (void*)zeroBitArgs) != 0) {
            fprintf(stderr, "Error creating zeroBitThread\n");
            exit(EXIT_FAILURE);
        }

        ThreadArgs* oneBitArgs = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        if (oneBitArgs == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        oneBitArgs->node = tail;

        if (pthread_create(&oneBitThread, NULL, countOneBits, (void*)oneBitArgs) != 0) {
            fprintf(stderr, "Error creating oneBitThread\n");
            exit(EXIT_FAILURE);
        }

        pthread_join(zeroBitThread, NULL);
        pthread_join(oneBitThread, NULL);

        Node* temp = current;
        current = current->next;
        free(temp); // Free the node after processing it

        temp = tail;
        tail = tail->prev;
        free(temp); // Free the node after processing
    }

    // Process remaining middle node if the list has odd number of elements
    if (current == tail) {
        if (current != NULL) {
            ThreadArgs* zeroBitArgs = (ThreadArgs*)malloc(sizeof(ThreadArgs));
            if (zeroBitArgs == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            zeroBitArgs->node = current;

            if (pthread_create(&zeroBitThread, NULL, countZeroBits, (void*)zeroBitArgs) != 0) {
                fprintf(stderr, "Error creating zeroBitThread\n");
                exit(EXIT_FAILURE);
            }

            pthread_join(zeroBitThread, NULL);

            free(current); // Free the node after processing it
        }
    }

    printf("Total zero bits in the list: %d\n", zeroBitCount);
    printf("Total one bits in the list: %d\n", oneBitCount);
    printf("Total zero bit nodes counted: %d\n", zeroBitNodesCount);
    printf("Total one bit nodes counted: %d\n", oneBitNodesCount);

    return 0;
}
