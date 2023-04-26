#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

char *get_path(char *name, char *program_name);

void get_input(char **buffer, size_t *bufsize, ssize_t *getret)
{
	signal(SIGINT, SIG_IGN);
	if (isatty(0) == 1)
	{
		printf("$ ");
	}
	*getret = getline(buffer, bufsize, stdin);
	fflush(stdin);
	if (*getret == -1)
	{
		free(*buffer);
		exit(EXIT_SUCCESS);
	}
}

int print_env(char *envp[])
{
	int i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i = i + 1;
	}
	return (0);
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
	extern char **environ;
	int status;
	
	while (1)
	{
		buffer = NULL;
		bufsize = 0;
		get_input(&buffer, &bufsize, &getret);
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
		if (strcmp(argv[0], "env") == 0)
		{
			print_env(environ);
			free(str);
			continue;
		}
		if (strcmp(argv[0], "exit") == 0)
		{
			free(str);
			exit(2);
		}
		if (stat(argv[0], &st) == 0 && st.st_mode & S_IXUSR)
		{
			returnpid = fork();
			if (returnpid == 0)
			{
				if (execve(argv[0], argv, environ) == -1)
				{
					printf("inside child exe failed %s: %s\n", av[0], strerror(errno));
					free(str);
					exit(2);
				}
			}
			else
			{
				wait(NULL);
			}
		}
		else if ((argv[0] = get_path(argv[0], av[0])) != NULL)
		{
			returnpid = fork();
			if (returnpid == 0)
			{
				if (execve(argv[0], argv, environ) == -1)
				{
					fprintf(stderr,"%s: %s: %s: not found\n", av[0], "1", argv[0]);
					free(str);
					free(argv[0]);
					exit(2);
				}
			}
			else
			{
				wait(&status);
				if (WIFEXITED(status))
				{
					status = WEXITSTATUS(status);
				}
			}
		}
		free(str);
	}
	return(status);
}
