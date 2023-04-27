#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

char *get_path(char **name, char *program_name);

int is_executable(char *string)
{
	struct stat st;

	if (stat(string, &st) == 0 && st.st_mode & S_IXUSR)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

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

void tokenise(char *str, char *argv[])
{
	int i;
	const char *delim;
	char *token;

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
}

int main(__attribute__((unused)) int ac, char *av[])
{
	char *argv[100];
	char *buffer;
	size_t bufsize;
	int returnpid;
	char * str;
	int k;
	ssize_t getret = 0;
	extern char **environ;
	int status;
	
	while (1)
	{
		k = 0;
		buffer = NULL;
		bufsize = 0;
		get_input(&buffer, &bufsize, &getret);
		str = strdup(buffer);
		free(buffer);
		tokenise(str, argv);
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
		if (is_executable(argv[0]) == 0)
		{
			argv[0] = get_path(&(argv[0]), av[0]);
			k = 1;
			if (argv[0] == NULL)
			{
				free(str);
				continue;
			}
		}
		returnpid = fork();
		if (returnpid == 0)
		{
			if (execve(argv[0], argv, environ) == -1)
			{
				printf("EXECVE FAIL\n");
				free(str);
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
		if (k == 1)
		{
			free(argv[0]);
		}
		free(str);
	}
	exit(127);
}
