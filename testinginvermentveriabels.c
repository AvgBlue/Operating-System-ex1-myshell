#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int array_lengths(char *envp[])
{
	int i = 0;
	while (envp[i++] != NULL)
		;
	return i;
}

int main()
{

	char *args[] = {"tennis.sh", NULL};
	setenv("PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/home/david/git_Repository_Drop_Place/Bash-tennis", 1);

	pid_t pidfork = fork();
	if (pidfork == -1)
	{
		// Fork failed
		perror("fork failed");
		return;
	}
	else if (pidfork == 0)
	{

		execvp(args[0], args);
		perror("execvp failed"); // If execvp returns, it means it failed
		exit(EXIT_FAILURE);		 // Exit child process with failure status
	}
	else
	{
		// Parent process
		int status;
		wait(&status); // Wait for child process to complete
		printf("you are stupid\n");
		printf("you are stupid\n");
		printf("you are stupid\n");
		printf("you are stupid\n");
		printf("you are stupid\n");
		printf("you are stupid\n");
	}
	return 0;
}

// // Append the values of corresponding environment variables to envp
// append_to_envp(envp);

// // Print the updated environment variables
// for (int i = 0; envp[i] != NULL; i++) {
//     printf("%s\n", envp[i]);
// }

// // Free allocated memory
// for (int i = 0; envp[i] != NULL; i++) {
//     free(envp[i]);
// }

// Define the environment variables
// char *envp[] = {
//     "PATH=/home/david/git_Repository_Drop_Place/Bash-tennis",
//     NULL
// };