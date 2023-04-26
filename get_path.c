#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void split_string_into_words(char* string, char** words);

/**
 * get_path - find the path of a file if it exist in PATH variable
 * @name: a string of the name of program to search for
 *
 * Return: if found, a file path name as a string is returned, otherwise NULL.
 */
char *get_path(char *name)
{
	int i;
	char *string;
	char *paths;
	char *each_path[100];
	size_t n;
	char *env_key;
	DIR *dp;
	struct dirent *ep;
	extern char **environ;
	char *return_string = NULL;

	env_key = "PATH";
	n = strlen(env_key);
	i = 0;
	while (environ[i] != NULL)
	{
		if (strncmp(environ[i], env_key, n) == 0)
		{
			string = strchr(environ[i], '=');
			paths = strdup(string + 1);
			split_string_into_words(paths, each_path);
		}
		i = i + 1;
	}
	i = 0;
	while (each_path[i] != NULL)
	{
		dp = opendir(each_path[i]);
		if (dp == NULL)
		{
			perror ("Couldn't open the directory");
		}
		ep = readdir(dp);
		if (ep == NULL)
		{
			printf("Error: Could not read directory\n");
		}
		while (ep != NULL)
		{
			if (strcmp(name, ep->d_name) == 0)
			{
				return_string = malloc(sizeof(*return_string) * (strlen(name) + 1 + strlen(each_path[i])));
				return_string = strcat(each_path[i], "/");
				return_string = strcat(return_string, ep->d_name);
				closedir(dp);
				free(paths);
				return (return_string);
			}
			ep = readdir(dp);
		}
		closedir (dp);
		i = i + 1;
	}
	free(paths);
	return (NULL);
}

void split_string_into_words(char* string, char** words)
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