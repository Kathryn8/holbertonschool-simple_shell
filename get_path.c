#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void split_string_into_words(char *string, char **words);
char *get_env_string(char *env_key);
DIR *_opendir(char *name);
struct dirent *_readdir(DIR *dp);
char *get_executable_string(char *path, char *program_name);

/**
 * get_path - find the path of a file if it exist in PATH variable
 * @name: a string of the name of program to search for
 * @program_name: name of the parent parent program for error message purposes
 *
 * Return: if found, a file path name as a string is returned, otherwise NULL.
 */
char *get_path(char **name, char *program_name)
{
	int i;
	char *string, *paths;
	char *each_path[100];
	DIR *dp;
	struct dirent *ep;

	string = get_env_string("PATH=");
	if (string == NULL)
	{
		fprintf(stderr, "%s: %s: %s: not found\n", program_name, "1", *name);
		return (NULL);
	}
	paths = strdup(string + 1);
	split_string_into_words(paths, each_path);
	i = 0;
	while (each_path[i] != NULL)
	{
		dp = _opendir(each_path[i]);
		ep = _readdir(dp);
		while (ep != NULL)
		{
			if (strcmp(*name, ep->d_name) == 0)
			{
				*name = get_executable_string(each_path[i], ep->d_name);
				free(paths);
				closedir(dp);
				return (*name);
			}
			ep = _readdir(dp);
		}
		closedir(dp);
		i = i + 1;
	}
	free(paths);
	fprintf(stderr, "%s: %s: %s: not found\n", program_name, "1", *name);
	return (NULL);
}

/**
 * split_string_into_words - create an array of strings from a string
 * @string: pointer to a string
 * @words: an array of strings or tokens returned from strtok
 *
 * Return: void
 */
void split_string_into_words(char *string, char **words)
{
	char *word;
	int i;

	i = 0;
	word = strtok(string, ":");
	while (word != NULL)
	{
		words[i] = word;
		i = i + 1;
		word = strtok(NULL, ":");
	}
	words[i] = NULL;
}

/**
 * get_env_string - takes an environment key and returns corresponding value
 * @env_key: a string to search against environment variables
 *
 * Return: corresponding value as a string, or NULL if no match is found
 */
char *get_env_string(char *env_key)
{
	int i, n;
	char *string;
	extern char **environ;

	n = strlen(env_key);
	i = 0;
	while (environ[i] != NULL)
	{
		if (strncmp(environ[i], env_key, n) == 0)
		{
			string = strchr(environ[i], '=');
			return (string);
		}
		i = i + 1;
	}
	return (NULL);
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

/**
 * get_executable_string - manipulate strings to create an executable
 * @path: a string of the name of the path
 * @program_name: name of the executable program
 *
 * Return: a string that is now executable
 */
char *get_executable_string(char *path, char *program_name)
{
	char *string;
	size_t size;

	size = sizeof(*string) * (strlen(program_name) + 2 + strlen(path));
	string = calloc(size, 1);
	if (string == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	strcat(string, path);
	strcat(string, "/");
	strcat(string, program_name);
	return (string);
}
