#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define WHITESPACE " \r\n\t\v\f"
#define CHANGEDIRECTORY "cd"

char** parse_command(char* line)
{
	int num_args = 0; // Record the number of arguments in each command.
	char* temp;
	char** output_str = malloc(sizeof(char*));

	if ( output_str == NULL )
	{
		perror("");
		exit(EXIT_FAILURE);
	}
	temp = strtok(line, WHITESPACE);
	// Remove all whitespace characters.
	if ( temp == NULL )
	// If it is an empty command (just containing whitespace characters).
	{
		return NULL;
	}
	while ( temp != NULL )
	{
		output_str[num_args] = temp;
		num_args++;
		output_str = realloc(output_str, (num_args + 1) * sizeof(char*));
		if ( output_str == NULL )
		{
			perror("");
			exit(EXIT_FAILURE);
		}
		temp = strtok(NULL, WHITESPACE);
	}
	num_args++;
	output_str = realloc(output_str, (num_args + 1) * sizeof(char*));
	// Create an extra slot for null character.
	if ( output_str == NULL )
	{
		perror("");
		exit(EXIT_FAILURE);
	}
	output_str[num_args] = NULL;
	return output_str;
}

void execute_command(char** output_str)
{
	pid_t childPID;
	int child_status;
	// Variables above are used in fork().
	if ( strcmp(output_str[0], CHANGEDIRECTORY) )
	{
		childPID = fork();
		if ( childPID >= 0 )
		{
			if ( childPID == 0 ) // Child process.
			{
				if ( execvp(output_str[0], output_str) == -1 )
				{
					perror(output_str[0]);
					exit(EXIT_FAILURE);
				}
			}
			else // Parent process.
			{
				wait(&child_status);
			}
		}
		else
		{
			perror("");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int dir;
		if ( output_str[1] == NULL )
		{
			dir = chdir(getenv("HOME"));
		}
		else
		{
			dir = chdir(output_str[1]);
		}
		if ( dir == -1 )
		{
			fprintf(stderr, "cd: %s: ", output_str[1]);
			perror("");
		}
	}
}
