#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define WHITESPACE " \r\n\t\v\f"
#define CHANGEDIR "cd"

char** parse_command(char* command_line)
{
	int num_args = 0; // Record the number of arguments in each command.
	char* temp;
	char** arg_vectors = malloc(sizeof(char*));

	if ( arg_vectors == NULL )
	{
		perror("");
		exit(EXIT_FAILURE);
	}
	temp = strtok(command_line, WHITESPACE);
	// Remove all whitespace characters.
	if ( temp == NULL )
	// If it is an empty command (just containing whitespace characters).
	{
		return NULL;
	}
	do
	{
		arg_vectors[num_args] = temp;
		num_args++;
		arg_vectors = realloc(arg_vectors, (num_args + 1) * sizeof(char*));
		if ( arg_vectors == NULL )
		{
			perror("");
			exit(EXIT_FAILURE);
		}
		temp = strtok(NULL, WHITESPACE);
	}
	while ( temp != NULL );
	arg_vectors[num_args] = NULL;
	// The list of arguments must be terminated by a NULL pointer.
	return arg_vectors;
}

void execute_command(char** arg_vectors)
{
	pid_t childPID;
	int child_status;
	// Variables above are used in fork().

	if ( strcmp(arg_vectors[0], CHANGEDIR) )
	{
		childPID = fork();
		if ( childPID >= 0 )
		{
			if ( childPID == 0 ) // Child process.
			{
				if ( execvp(arg_vectors[0], arg_vectors) == -1 )
				{
					perror(arg_vectors[0]);
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
		if ( arg_vectors[1] == NULL )
		{
			dir = chdir(getenv("HOME"));
		}
		else
		{
			dir = chdir(arg_vectors[1]); // Ignore rest of arguments.
		}
		if ( dir == -1 ) // If the program cannot change working directory.
		{
			fprintf(stderr, "%s: %s: ", CHANGEDIR, arg_vectors[1]);
			perror("");
		}
	}
}
