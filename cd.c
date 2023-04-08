#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	char directory[4096] = " ";
	while (strcmp(directory, "exit") != 0)
	{
		scanf(" %s", &directory);
		chdir(directory);
		getcwd(directory, sizeof(directory));
		printf("%s\n", directory);
	}

	return 0;
}
