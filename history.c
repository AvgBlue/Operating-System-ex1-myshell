#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100 // Maximum size of the queue
#define MAX_LENGTH 100 // Maximum length of each string in the queue

char queue[MAX_SIZE][MAX_LENGTH + 1]; // Queue data structure
int front = 0; // Front pointer
int rear = -1; // Rear pointer
int count = 0; // Count of elements in the queue

// Function to push a string into the queue
void push(char value[]) {
    if (rear == MAX_SIZE - 1) {
        printf("Queue is full. Cannot push element.\n");
    } else {
        rear++;
        strncpy(queue[rear], value, MAX_LENGTH);
        count++;
    }
}

// Function to pop a string from the queue
void pop() {
    if (front > rear) {
        printf("Queue is empty. Cannot pop element.\n");
    } else {
        printf("Popped element: %s\n", queue[front]);
        front++;
        count--;
    }
}

// Function to count the number of elements in the queue
int queueCount() {
    return count;
}

// Function to print all elements in the queue
void printQueue() {
    if (front > rear) {
        printf("Queue is empty. No elements to print.\n");
    } else {
        printf("Queue elements: \n");
        for (int i = front; i <= rear; i++) {
            printf("%s\n", queue[i]);
        }
    }
}

void addToHistory(char* queue){
	if(queueCount()==100){
		pop();
	}
	push(queue);
}



int main() {
    // Example usage
    push("one");
    push("two");
    push("three");
    printf("Queue count: %d\n", queueCount()); // Output: Queue count: 3
    printQueue(); // Output: Queue elements: one two three
    pop(); // Output: Popped element: one
    printQueue(); // Output: Queue elements: two three

    return 0;
}
