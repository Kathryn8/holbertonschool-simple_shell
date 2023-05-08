#include "main.h"

/**
 * main - a program to run a simple shell
 * @ac: user input count, unused for this program
 * @av: array of user inputs, index 0 is program name
 *
 * Return: 0 on success, 127 if program not found
 */
int main(__attribute__((unused)) int ac, char *av[])
{
	char *argv[100];
	char *buffer;
	size_t bufsize;
	char *str;
	int k, exit_status, status;
	ssize_t getret = 0;

	status = 0;
	while (1)
	{
		k = 0;
		buffer = NULL;
		bufsize = 0;
		get_input(&buffer, &bufsize, &getret);
		str = strdup(buffer);
		free(buffer);
		assign_words_to_array(str, argv);
		if (builtin_commands(argv, environ, &status, str) == 1)
		{
			continue;
		}
		if (is_executable(argv[0]) == 0)
		{
			argv[0] = get_path(&(argv[0]), av[0]);
			k = 1;
			if (argv[0] == NULL)
			{
				free(str);
				return (127);
			}
		}
		exit_status = fork_child_adult(argv, environ, &str);
		if (k == 1)
		{
			free(argv[0]);
		}
		free(str);
	}
	return (exit_status);
}
