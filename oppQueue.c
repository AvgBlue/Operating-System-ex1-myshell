#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAX_SIZE 5			// Maximum size of the queue
#define MAX_STRING_SIZE 100 // Maximum size of a string

typedef struct
{
	char queue[MAX_SIZE][MAX_STRING_SIZE + MAX_STRING_SIZE]; // Queue data structure
	int front;												 // Front pointer
	int rear;												 // Rear pointer
	int count;												 // Count of elements in the queue
} Queue;

// Function to initialize the queue
void initQueue(Queue *q)
{
	q->front = 0;
	q->rear = -1;
	q->count = 0;
}

// Function to push a string into the queue
void push(Queue *q, char *value)
{
	if (q->count == MAX_SIZE)
	{
		return;
	}
	q->rear = (q->rear + 1) % MAX_SIZE; // Update rear pointer with circular buffer approach
	strcpy(q->queue[q->rear], value);
	q->count++;
}

// Function to pop a string from the queue
void pop(Queue *q)
{
	if (q->count == 0)
	{
		return;
	}
	q->front = (q->front + 1) % MAX_SIZE; // Update front pointer with circular buffer approach
	q->count--;
}

// Function to count the number of elements in the queue
int queueCount(Queue *q)
{
	return q->count;
}

// Function to print all elements in the queue
void printQueue(Queue *q)
{
	if (q->count == 0)
	{
		return;
	}
	int i = q->front;
	int elementsPrinted = 0;
	while (elementsPrinted < q->count)
	{
		printf("%s\n", q->queue[i]);
		i = (i + 1) % MAX_SIZE; // Update index with circular buffer approach
		elementsPrinted++;
	}
}

// Function to add a string to the history queue
void addToHistory(Queue *q, pid_t pid, char *str)
{
	if (queueCount(q) == MAX_SIZE)
	{
		pop(q);
	}
	char value[MAX_STRING_SIZE] = " ";
	sprintf(value, "%d %s", pid, str);
	push(q, value);
}

int main()
{
	// Example usage
	Queue q;
	initQueue(&q);
	addToHistory(&q, 1, "apple");
	addToHistory(&q, 2, "banana");
	addToHistory(&q, 3, "cherry");
	addToHistory(&q, 4, "apple");
	addToHistory(&q, 5, "banana");
	printQueue(&q);
	printf("\n");
	addToHistory(&q, 6, "cherry");
	addToHistory(&q, 7, "apple");
	printQueue(&q); // Output: Queue elements: banana cherry

	return 0;
}
