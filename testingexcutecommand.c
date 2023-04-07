#include <stdio.h>
#include <string.h>

void executeCommand(char arr[][100], int numWords);

int main() {
    char command[1][100]; // Declare an array to hold the command
    strcpy(command[0], "echo"); // Set the first element to the command "echo"
    strcpy(command[1], "I'm here"); // Set the second element to the argument "I'm here"
    executeCommand(command, 2); // Call the executeCommand function with the command and argument
    return 0;
}
