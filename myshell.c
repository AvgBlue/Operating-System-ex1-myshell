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
void pop(Queue *q) {
    if (q->count == 0) {
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
 * wordCount - Function to count the number of words in a string.
 *
 * @param str: Input string to count the words.
 *
 * @return: The number of words found in the input string.
 *
 * @details: This function takes an input string and counts the number of words in it.
 *           Words are considered as consecutive non-space characters separated by spaces.
 *           The function uses a flag to keep track of whether the current character is part
 *           of a word or not, and increments the word count whenever a new word is found.
 *           The word count is returned as the result.
 *
 * @note: The input string 'str' should be a null-terminated string. The function does not
 *        modify the input string, and it does not consider any leading or trailing spaces
 *        as part of a word.
 */
int wordCount(const char *str)
{
	int count = 0;
	int len = strlen(str);
	int isWord = 0; // flag to track if current character is part of a word

	for (int i = 0; i < len; i++)
	{
		// Check if current character is not a space and previous character is a space or it's the beginning of the string
		if (str[i] != ' ' && (i == 0 || str[i - 1] == ' '))
		{
			isWord = 1; // Set flag to indicate current character is part of a word
			count++;	// Increment word count
		}
		else if (str[i] == ' ')
		{
			isWord = 0; // Reset flag when space is encountered
		}
	}

	return count;
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

int executeCommand(char arr[][MAX_STRING_SIZE], int numWords)
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
	return pidfork;
}

int main()
{
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
		// cd implumention
		if (strcmp(arr[0], "cd") == 0)
		{
			// not enath arguments
			if (numWords > 2)
			{
				printf("Error for too many argument in CD\n");
				addToHistory(&historyQueue, getpid(), str);
				continue;
			}
			// spricell case
			if (strcmp(arr[1], ".") == 0 || strcmp(arr[1], "..") == 0)
			{
				chdir(arr[1]);
				addToHistory(&historyQueue, getpid(), str);
				continue;
			}
			// regular try to get in
			if (chdir(arr[1]) == NULL)
			{
				addToHistory(&historyQueue, getpid(), str);
				printf("Error in cd directory selection\n");
			}
			continue;
		}
		// history implumention
		if (strcmp(arr[0], "history") == 0)
		{
			addToHistory(&historyQueue, getpid(), str);
			printHistory(&historyQueue);
			continue;
		}
		pid_t pid = executeCommand(arr, numWords);
		addToHistory(&historyQueue, pid, str);
	}
	return 0;
}