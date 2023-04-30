#include "main.h"

/**
 * builtin_commands - checks if command received matches with a built in
 * command, and runs the command if there is a match
 * @argv: command received
 * @environ: environment of the current process
 * @status: exit status number
 * @str: pointer to string originally entered into the prompt
 *
 * Return: 1 if built in command found, else 0.
 */

int builtin_commands(char **argv, char **environ, int *status, char *str)
{
	if (argv[0] == NULL)
	{
		free(str);
		return (1);
	}
	if (strcmp(argv[0], "env") == 0)
	{
		print_env(environ);
		free(str);
		return (1);
		}
	if (strcmp(argv[0], "exit") == 0)
	{
		free(str);
		*status = 2;
		exit(*status);
	}
	return (0);
}

/**
 * print_env - prints the current environment
 * @envp: array of pointers to environment variables
 *
 * Return: 0 always.
 */
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

/**
 * _opendir - custom opendir function
 * @name: directory name
 *
 * Return: pointer to directory stream
 */
DIR *_opendir(char *name)
{
		DIR *dp;

		dp = opendir(name);
		if (dp == NULL)
		{
			perror("open directory");
			return (NULL);
		}
		return (dp);
}

/**
 * _readdir - custom readdir function
 * @dp: pointer to a directory stream
 *
 * Return: pointer to a directory entry struct
 */
struct dirent *_readdir(DIR *dp)
{
	struct dirent *ep;

	ep = readdir(dp);
	if (ep == NULL)
	{
		return (NULL);
	}
	return (ep);
}
