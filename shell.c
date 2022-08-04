#include "main.h"

/**
 * main - shell main func
 * @ac: unused
 * @argv: pointer list of args
 * Return: 0
 */
int main(void)
{
	size_t buffsize;
	char *buff, **args;
	int tty = 0, weGood = 1;

	tty = isatty(STDIN_FILENO);

	while (weGood)
	{
		if (tty == 1)
			printf("$ ");

		while(getline(&buff, &buffsize, stdin))
		{
			args = prep_string(buff);

			while (args)
			{
				printf("%s\n", args[0]);
				args++;
			}
			/* prepare arguments for checks */

			/* check builtins */

			/* check path */

			/* execute command (only if found in path? */
		}
		weGood = 0;


	}
	return (0);
}
/**
 * shell - high level shell loop
 *
 * @argv: pointer array of args
 * @tty: is/is not tty
 * Return: 0
 */
int shell(char **argv, int tty)
{
	size_t buffsize = 0;
	char *buff = NULL, *tmp = NULL; /**cmd = NULL*/
	int valid_input = 1;


	while (valid_input)
	{
		if (tty == 1)
			printf("$ ");

		valid_input = getline(&buff, &buffsize, stdin);
		
		if (valid_input > 0)
		{
			printf("buff: %s\n", buff);
			if (_strcmp(buff, "exit\n") == 0)
			{
				break;
			}
		/* 	tmp = prep_string(buff);  */

			if (tmp == NULL)
				perror(argv[0]);
			/*
		 *cmd = is_cmd_exist(tmp);
		 *if (cmd == NULL)
		 *	perror(argv[0]);
		 *else
		 * {
		 *	command(cmd);
		 *	if (_strcmp(tmp, cmd) != 0)
		 *		free(cmd);
		 * }
		*/
			command(tmp);
		}
	}
	free(buff);
	buff = NULL;
	return (0);
}

/**
 * prep_string - takes a string, replaces newline with null byte
 *
 * @cmd: command to prep
 * Return: pointer to first char of cmd
 */
char **prep_string(char *buff)
{
	char *args[32], *tok;
	int i = 0;
	
	while (buff[0] == ' ')
		buff++;

	tok = strtok(buff, " ");

	while (tok != NULL)
	{
		args[i] = tok;
		tok = strtok(buff, " \n");
		i++;
	}
	args[i] = NULL;

	return(args);
}

/**
 * is_cmd_exist - check if cmd exists in path or current directory
 * * @cmd: cmd to check
 * Return: pointer to cmd
 */
char *is_cmd_exist(char *cmd)
{
	struct stat st;
	char *env_path_var, *arg, *full_path;

	if (stat(cmd, &st) == 0)
		return (cmd);

	env_path_var = _strdup(getenv("PATH")); /* getenv - not allowed */

	arg = strtok(env_path_var, ":");

	while (arg != NULL)
	{
		full_path = strcpycat(arg, cmd);
		if (stat(full_path, &st) == 0)
		{
			/* found path */
			free(env_path_var);
			return (full_path);
		}

		arg = strtok(NULL, ":\0\n");
		free(full_path);
	}

	free(env_path_var);
	return (NULL);
}

/**
 * command - execute cmd
 * @cmd: command to execute
 * Return: 0
 */
int command(char *cmd)
{
	pid_t my_pid;
	char *argv[2];
	struct stat st;

	argv[0] = cmd;
	argv[1] = NULL;


	if (stat(cmd, &st) != 0)
	{
		perror(argv[0]);
		return (0);
	}
	my_pid = fork();
	if (my_pid == -1)
	{
		return (EXIT_FAILURE);
	}
	if (my_pid == 0)
	{
		if (execve(argv[0], argv, NULL) == -1)
		{
			perror("Error: execve failed in command function.");
			exit(99);
		}
	}
	else
	{
		wait(NULL);
	}

	return (EXIT_SUCCESS);
}
