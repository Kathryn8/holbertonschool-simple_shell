#include "main.h"

/**
 * is_executable - checks if command is executable, and if user has the
 * correct permissions to execute the file
 * @string: command to be checked
 *
 * Return: 1 if executable with permissions for user to execute, 0 if not.
 */
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

/**
 * get_input - receives a prompt from the stdin, and writes it to a buffer.
 * @buffer: name of buffer, that the prompt is written in to
 * @bufsize: size of the buffer
 * @getret: number of chracters read, or -1 on failure to read a line
 * or end of file condition (EOF)
 *
 * Return: void
 */
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

/**
 * assign_words_to_array - receives a string and separates each word, then
 * assigns each word into an array
 * @str: string to be split into words
 * @argv: name of array that the words will be assigned to
 *
 * Return: nothing.
 */
void assign_words_to_array(char *str, char *argv[])
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

/**
 * fork_child_adult - creates a child process, then executes a command within.
 * @argv: command to be executed
 * @environ: environment variables of the current process
 * @str: pointer to string originally entered into the prompt
 *
 * Return: the exit status of the child process
 */
int fork_child_adult(char **argv, char **environ, char **str)
{
	int pidreturn;
	int status;
	int exit_status = 0;

	pidreturn = fork();
	if (pidreturn == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			printf("EXECVE FAIL\n");
			free(*str);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
		}
	}
	return (exit_status);
}

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
	int k, exit_status;
	ssize_t getret = 0;

	while (1)
	{
		k = 0;
		buffer = NULL;
		bufsize = 0;
		get_input(&buffer, &bufsize, &getret);
		str = strdup(buffer);
		free(buffer);
		assign_words_to_array(str, argv);
		if (builtin_commands(argv, environ, str) == 1)
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
