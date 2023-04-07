#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

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
int wordCount(const char *str) {
    int count = 0;
    int len = strlen(str);
    int isWord = 0; // flag to track if current character is part of a word

    for (int i = 0; i < len; i++) {
        // Check if current character is not a space and previous character is a space or it's the beginning of the string
        if (str[i] != ' ' && (i == 0 || str[i - 1] == ' ')) {
            isWord = 1; // Set flag to indicate current character is part of a word
            count++; // Increment word count
        } else if (str[i] == ' ') {
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
void stringToArray(const char str[], char arr[][100], int *numWords) {
    char copyStr[100]; // Make a copy of the input string to avoid modifying it
    strcpy(copyStr, str);

    char *token = strtok(copyStr, " "); // Tokenize the input string by space

    // Copy each token (word) to the array
    while (token != NULL && *numWords < 100) {
        strcpy(arr[*numWords], token);
        (*numWords)++;
        token = strtok(NULL, " ");
    }
}

void inputCommand(char str[100]){
	printf("$ "); 
	fflush(stdout);
	scanf(" %100[^\n\r]", str);
}

void executeCommand(char arr[][100], int numWords) {
    pid_t pidfork = fork();
    if (pidfork == -1) {
        // Fork failed
        perror("fork failed");
        return;
    } else if (pidfork == 0) {
        // Child process
      	char** args = (char**)calloc(numWords + 1, sizeof(char*)); // Allocate memory for arguments array
        for (int i = 0; i < numWords; i++) {
            args[i] = arr[i]; // Assign each word to the arguments array
        }
        args[numWords] = NULL; // Set last element of arguments array to NULL as required by execvp
        execvp(args[0], args); // Execute the command
        perror("execvp failed"); // If execvp returns, it means it failed
        exit(EXIT_FAILURE); // Exit child process with failure status
    } else {
        // Parent process
        int status;
        wait(&status); // Wait for child process to complete
    }
}



int main () {
	char str[100]=" ";
	
	while(1){
		inputCommand(str);
		if (strcmp(str,"exit") == 0){
			break;
		}
		printf("your input is %s\n",str);
	}
	return 0;
}