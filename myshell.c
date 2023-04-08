#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define INPUT_SIZE 100
#define EXIT_EXECVP_FAILED 99
#define MAX_SIZE 100		// Maximum size of the queue
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
void printHistory(Queue *q)
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

/**
 * stringToArray - Function to separate a string into words and store them in a 2D array.
 *
 * @param str: Input string to be separated into words.
 * @param arr: 2D array to store the separated words.
 * @param numWords: Pointer to an integer to store the number of words found.
 *
 * @return: None.
 *
 * @details: This function takes an input string, uses strtok to tokenize it by space,
 *           and stores the resulting words in a 2D array. The number of words found is
 *           stored in the numWords parameter, which is passed as a pointer to allow
 *           updating its value within the function.
 *
 * @note: The input string 'str' should be a null-terminated string with a maximum
 *        length of 100 characters. The 2D array 'arr' should have a size of [100][100],
 *        where each word is stored in a row with a maximum length of 100 characters.
 *        The 'numWords' parameter should be initialized to 0 before calling the function,
 *        and it will be updated with the number of words found in the input string.
 */
void stringToArray(const char str[], char arr[][MAX_STRING_SIZE], int *numWords)
{
	char copyStr[100]; // Make a copy of the input string to avoid modifying it
	strcpy(copyStr, str);

	char *token = strtok(copyStr, " "); // Tokenize the input string by space

	// Copy each token (word) to the array
	while (token != NULL && *numWords < 100)
	{
		strcpy(arr[*numWords], token);
		(*numWords)++;
		token = strtok(NULL, " ");
	}
}

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
		char **args = (char **)calloc(numWords + 1, sizeof(char *)); // Allocate memory for arguments array
		for (int i = 0; i < numWords; i++)
		{
			args[i] = arr[i]; // Assign each word to the arguments array
		}
		args[numWords] = NULL;	  // Set last element of arguments array to NULL as required by execvp
		execvp(args[0], args);	  // Execute the command
		perror("execvp failed");  // If execvp returns, it means it failed
		exit(EXIT_EXECVP_FAILED); // Exit child process with failure status
	}
	// Parent process
	int status;
	wait(&status); // Wait for child process to complete
	addToHistory(historyQueue, pidfork, str);
}

int cd(char str[MAX_STRING_SIZE], char arr[MAX_SIZE][MAX_STRING_SIZE], int numWords, Queue *historyQueue)
{
	// cd implumention
	if (strcmp(arr[0], "cd") == 0)
	{
		addToHistory(historyQueue, getpid(), str);
		// not enath arguments
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

	free(path); // Free the allocated memory for path
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