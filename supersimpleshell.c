#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void get_input(char **buffer, size_t *bufsize, ssize_t *getret)
{
	signal(SIGINT, SIG_IGN);
	if (isatty(0) == 1)
	{
		printf("$ ");
	}
	*getret = getline(buffer, bufsize, stdin);
	if (*getret == -1)
	{
		free(*buffer);
		exit(EXIT_SUCCESS);
	}
}

int main(__attribute__((unused)) int ac, char *av[])
{
	char *argv[100];
	char *buffer;
	size_t bufsize;
	int returnpid;
	const char *delim;
	char *token;
	char * str;
	int i;
	ssize_t getret = 0;
	struct stat st;
	
	while (1)
	{
		buffer = NULL;
		bufsize = 0;
		get_input(&buffer, &bufsize, &getret);
		str = malloc(sizeof(*str) * bufsize);
		str = strdup(buffer);
		free(buffer);
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
		if (argv[0] == NULL)
		{
			free(str);
			continue;
		}
		if (stat(argv[0], &st) == -1)
		{
			printf("%s: No such file or directory\n", av[0]);
			continue;
		}
		if (st.st_mode & S_IXUSR)
		{
			returnpid = fork();
			if (returnpid == 0)
			{
				if (execve(argv[0], argv, NULL) == -1)
				{
					printf("%s: No such file or directory\n", av[0]);
					return (-1);
				}
			}
		}
		else
		{
			wait(NULL);
		}
		free(str);
	}
}
