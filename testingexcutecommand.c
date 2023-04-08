#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>



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


void executeCommand(char arr[][100], int numWords) {
    pid_t pidfork = fork();
    if (pidfork == -1) {
        // Fork failed
        perror("fork failed");
        return;
    } else if (pidfork == 0) {
        // Child process
        char** args = (char**)malloc((numWords + 1) * sizeof(char*)); // Allocate memory for arguments array
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


int main() {
	char* array[]={"echo","\"number","one","number","two\"",NULL};
	execvp(array[0], array);
    return 0;
}
