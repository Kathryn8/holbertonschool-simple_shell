#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	char *argv[2];
	int returnpid;
	char *buffer;
	size_t bufsize;
	ssize_t getret;
	const char *delim;
	char *token;
	char * str;
	int i;

	while (1)
	{
		signal(SIGINT, SIG_IGN); // this stops the ctrl + C
		printf("$ ");
		buffer = NULL;
		bufsize = 0;
		getret = getline(&buffer, &bufsize, stdin);
		if (getret == -1)
		{
			free(buffer); //should we free here?
			return (0);
		}
		if (argv[0] == NULL)
		{
			printf("thats a space\n");
			continue;
		}

		str = malloc(sizeof(*str) * bufsize);
		str = buffer;
		delim = " \t\n";
		token = strtok(str, delim);
		i = 0;
		while (token != NULL)
		{
			argv[i] = token;
			token = strtok(NULL, delim);
			i = i + 1;
		}
		argv[i] = NULL;
		returnpid = fork();
		if (returnpid == 0)
		{
			
			if (execve(argv[0], argv, NULL) == -1)
			{
				printf("%s: No such file or directory\n", argv[0]);
				return (-1);
			}
		}
		else
		{
			wait(NULL);
		}
		free(buffer);
	}
}
