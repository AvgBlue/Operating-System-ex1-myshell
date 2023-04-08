#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define EXIT_EXECVP_FAILED 99
#define MAX_SIZE 100
#define MAX_STRING_SIZE 100

typedef struct
{
	char queue[MAX_SIZE][MAX_STRING_SIZE + MAX_STRING_SIZE];
	int front;
	int rear;
	int count;
} Queue;

//initializes the queue
void initQueue(Queue *q)
{
	//set front to 0
	q->front = 0;
	//set rear to -1
	q->rear = -1;
	//set count 0;
	q->count = 0;
}
// This function adds an element to the queue
void push(Queue *q, char *value)
{
	// If the queue is full, return
	if (q->count == MAX_SIZE)
	{
		return;
	}

	// Increment the rear pointer and add the value to the queue
	q->rear = (q->rear + 1) % MAX_SIZE;
	strcpy(q->queue[q->rear], value);
	q->count++;
}

// pop element from the queue
void pop(Queue *q)
{
    // Check if the queue is empty
    if (q->count == 0)
    {
        return; // If empty, return without making any changes
    }
    
    // Update the front index of the queue to remove the element
    q->front = (q->front + 1) % MAX_SIZE; // Circular buffer implementation
    
    // Decrease the count of elements in the queue
    q->count--;
}

// return the number of elements in the queue
int queueCount(Queue *q)
{
    // Return the count of elements in the queue
    return q->count;
}

//printing the history Queue
void printHistory(Queue *q)
{
    // Check if the queue is empty
    if (q->count == 0)
    {
		// If empty, return without printing anything
        return; 
    }
     // Initialize index to the front of the queue
    int i = q->front;
	// Initialize counter for elements printed
    int elementsPrinted = 0; 
    
    // Loop through the queue and print the elements
    while (elementsPrinted < q->count)
    {
		// Print the element at the current index
        printf("%s\n", q->queue[i]); 
        
		// Move to the next index, considering circular buffer
        i = (i + 1) % MAX_SIZE; 
        
        elementsPrinted++; // Increase the counter for elements printed
    }
}

//add elements to the history queue
void addToHistory(Queue *q, pid_t pid, char *str)
{
    if (queueCount(q) == MAX_SIZE)
    {
		// If the queue is full, remove the oldest element from the front
        pop(q); 
    }
	// Create a new string buffer for the value
    char value[MAX_STRING_SIZE] = " "; 
	// Format the process ID and string into the value buffer
    sprintf(value, "%d %s", pid, str); 
	// Add the new value to the end of the queue
    push(q, value); 
}


//convert the string to an array and return the number words the array
void stringToArray(const char str[], char arr[][MAX_STRING_SIZE], int *numWords)
{
    // Create a temporary copy of the input string to avoid modifying the original string
    char copyStr[100]; 
    strcpy(copyStr, str); 

    // Use strtok to tokenize the string by space (" ")
    char *token = strtok(copyStr, " "); 

    // Loop until all tokens (words) are processed or numWords reaches the maximum limit
    while (token != NULL && *numWords < 100) 
    {
        // Copy the token (word) into the array of strings at the current index
        strcpy(arr[*numWords], token); 
        // Increment the counter for number of words
        (*numWords)++; 
        // Move to the next token (word)
        token = strtok(NULL, " "); 
    }
}


//input the a command from the user
void inputCommand(char str[MAX_STRING_SIZE])
{
	printf("$ ");
	fflush(stdout);
	scanf(" %100[^\n\r]", str);
}

void executeCommand(char str[MAX_STRING_SIZE], char arr[MAX_SIZE][MAX_STRING_SIZE], int numWords, Queue *historyQueue)
{
	pid_t pidfork = fork();
	if (pidfork == -1)
	{
		// Fork failed
		perror("fork failed");
		return;
	}
	else if (pidfork == 0)
	{
		// Child process
		char **args = (char **)calloc(numWords + 1, sizeof(char *));
		for (int i = 0; i < numWords; i++)
		{
			args[i] = arr[i];
		}
		args[numWords] = NULL;
		execvp(args[0], args);
		perror("execvp failed");
		exit(EXIT_EXECVP_FAILED);
	}
	// Parent process
	int status;
	wait(&status);
	addToHistory(historyQueue, pidfork, str);
}

int cd(char str[MAX_STRING_SIZE], char arr[MAX_SIZE][MAX_STRING_SIZE], int numWords, Queue *historyQueue)
{
	// cd implumention
	if (strcmp(arr[0], "cd") == 0)
	{
		addToHistory(historyQueue, getpid(), str);

		if (numWords > 2)
		{
			printf("cd: too many arguments\n");
			return 1;
		}
		// spricell case
		if (strcmp(arr[1], ".") == 0 || strcmp(arr[1], "..") == 0)
		{
			chdir(arr[1]);
			return 1;
		}
		// regular try to get in
		if (chdir(arr[1]) == -1)
		{
			printf("history: too many arguments\n");
		}
		return 1;
	}
	return 0;
}

int history(char str[MAX_STRING_SIZE], char arr[MAX_SIZE][MAX_STRING_SIZE], int numWords, Queue *historyQueue)
{
	// history implumention
	if (strcmp(arr[0], "history") == 0)
	{
		addToHistory(historyQueue, getpid(), str);
		if (numWords > 1)
		{
			printf("Error for too many argument in CD\n");
			return 1;
		}
		printHistory(historyQueue);
		return 1;
	}
	return 0;
}

void settingPath(int argc, char *argv[])
{
	char *value = getenv("PATH");
	int path_size = strlen(value) + 1;
	char *path = (char *)calloc(path_size, sizeof(char));
	strcpy(path, value);

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "\0") == 0)
		{
			continue;
		}
		int size = strlen(path) + strlen(argv[i]) + 10;
		if (size >= path_size)
		{
			path = (char *)realloc(path, size * sizeof(char));
			path_size = size;
		}
		strcat(path, ":");
		strcat(path, argv[i]);
	}
	setenv("PATH", path, 1);

	free(path);
}

int main(int argc, char *argv[])
{
	settingPath(argc, argv);
	Queue historyQueue;
	initQueue(&historyQueue);
	while (1)
	{
		char str[MAX_STRING_SIZE] = " ";
		char arr[MAX_SIZE][MAX_STRING_SIZE];
		int numWords = 0;
		inputCommand(str);
		stringToArray(str, arr, &numWords);
		// exit implumention
		if (strcmp(arr[0], "exit") == 0)
		{
			break;
		}
		if (cd(str, arr, numWords, &historyQueue) || history(str, arr, numWords, &historyQueue))
		{
			continue;
		}
		executeCommand(str, arr, numWords, &historyQueue);
	}
	return 0;
}