#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100 // Maximum size of the queue
#define MAX_STRING_SIZE 100 // Maximum size of a string

typedef struct {
    char queue[MAX_SIZE][MAX_STRING_SIZE]; // Queue data structure
    int front; // Front pointer
    int rear; // Rear pointer
    int count; // Count of elements in the queue
} Queue;

// Function to initialize the queue
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

// Function to push a string into the queue
void push(Queue *q, char *value) {
    if (q->rear == MAX_SIZE - 1) {
        printf("Queue is full. Cannot push element.\n");
    } else {
        q->rear++;
        strcpy(q->queue[q->rear], value);
        q->count++;
    }
}

// Function to pop a string from the queue
void pop(Queue *q) {
    if (q->front > q->rear) {
        printf("Queue is empty. Cannot pop element.\n");
    } else {
        printf("Popped element: %s\n", q->queue[q->front]);
        q->front++;
        q->count--;
    }
}

// Function to count the number of elements in the queue
int queueCount(Queue *q) {
    return q->count;
}

// Function to print all elements in the queue
void printQueue(Queue *q) {
    if (q->front > q->rear) {
        printf("Queue is empty. No elements to print.\n");
    } else {
        printf("Queue elements: ");
        for (int i = q->front; i <= q->rear; i++) {
            printf("%s ", q->queue[i]);
        }
        printf("\n");
    }
}

// Function to add a string to the history queue
void addToHistory(Queue *q, char *value) {
    if (queueCount(q) == MAX_SIZE) {
        pop(q);
    }
    push(q, value);
}

int main() {
    // Example usage
    Queue q;
    initQueue(&q);

    push(&q, "apple");
    push(&q, "banana");
    push(&q, "cherry");
    printf("Queue count: %d\n", queueCount(&q)); // Output: Queue count: 3
    printQueue(&q); // Output: Queue elements: apple banana cherry
    pop(&q); // Output: Popped element: apple
    printQueue(&q); // Output: Queue elements: banana cherry

    return 0;
}
