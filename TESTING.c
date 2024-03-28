#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
// Structure of doubly linked list Node
typedef struct doublyLinkedlist {
    int data;
    struct doublyLinkedlist* next;
    struct doublyLinkedlist* prev; // Pointer to the previous node
} doublyLinkedlist;


doublyLinkedlist* mylist_head = NULL;
doublyLinkedlist* mylist_tail = NULL;
const int N;

// Function to create a new node with random data
doublyLinkedlist* createNode() {
    doublyLinkedlist* newNode = (doublyLinkedlist*)malloc(sizeof(doublyLinkedlist));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->data = rand() % 256;
    newNode->next = NULL;
    newNode->prev = NULL; // Initialize prev pointer
    return newNode;
}

// Function to free memory occupied by list
void freeList() {
    doublyLinkedlist* temp;
    while (mylist_head != NULL) {
        temp = mylist_head;
        mylist_head = mylist_head->next;
        free(temp);
    }
    mylist_tail = NULL; // Reset tail pointer
}

// Function for thread to traverse the list and process elements
void* thread_func(void* arg) {
    bool f = *(bool*)arg;
    unsigned int count = 0;
    int tempvalue;

    // Determine thread number
    int thread_num = f ? 1 : 2;

    while (count < N) {
        pthread_mutex_lock(&my_mutex);
        if (mylist_head == NULL) {
            pthread_mutex_unlock(&my_mutex);
            continue;
        }

        tempvalue = mylist_head->data;
        doublyLinkedlist* temp = mylist_head;
        mylist_head = mylist_head->next;
        if (mylist_head != NULL) {
            mylist_head->prev = NULL; // Update the previous node's prev pointer
        } else {
            mylist_tail = NULL; // Reset tail pointer if the list becomes empty
        }
        count++;
        pthread_mutex_unlock(&my_mutex);

        // Process tempvalue here (count zero or one bits)

        if (f) {
            // count zero bits
            int zeroBitsCount = 0;
            int num = tempvalue;
            while (num) {
                if ((num & 1) == 0)
                    zeroBitsCount++;
                num >>= 1;
            }
            printf("Thread %d: Element: %d, Zero bits count: %d\n", thread_num, tempvalue, zeroBitsCount);
        } else {
            // count one bits
            int oneBitsCount = 0;
            int num = tempvalue;
            while (num) {
                if (num & 1)
                    oneBitsCount++;
                num >>= 1;
            }
            printf("Thread %d: Element: %d, One bits count: %d\n", thread_num, tempvalue, oneBitsCount);
        }

        free(temp); // Free the processed node
    }

    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    bool flag1 = true, flag2 = false;

    // Creating the doubly linked list
    printf("%s","Enter the number of Nodes : \n");
    scanf("%d",&N);
    for (int i = 0; i < N; i++) {
        doublyLinkedlist* newNode = createNode();
        printf("%d \n", newNode->data);
        newNode->next = mylist_head;
        if (mylist_head != NULL) {
            mylist_head->prev = newNode; // Update the previous node's prev pointer
        } else {
            mylist_tail = newNode; // Set tail pointer for the first node
        }
        mylist_head = newNode;
    }


    // Creating threads
    pthread_create(&thread1, NULL, thread_func, &flag1);
    pthread_create(&thread2, NULL, thread_func, &flag2);

    // Waiting for threads to finish execution
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Freeing the memory
    freeList();

    return 0;
}
