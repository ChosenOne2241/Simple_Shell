#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PS1 "[%s] # "
#define TIME_FORMAT "%d/%m %R"
#define BUFFER_SIZE 40

void print_prompt(void);

int main(void)
{
	size_t len = 0;
	ssize_t read;
	char* line = NULL;

	pid_t childPID;
	int child_status;

	int count;
	char* temp;
	char** output_str = malloc(sizeof(char*));
	if ( output_str == NULL )
	{
		perror("");
		exit(EXIT_FAILURE);
	}

	print_prompt();
	while ( (read = getline(&line, &len, stdin)) != -1 )
	{
		// system(line);

		count = 0;
		temp = strtok(line, " \n");
		while ( temp != NULL )
		{
			output_str[count] = temp;
			count++;
			output_str = realloc(output_str, (count + 1) * sizeof(char*));
			if ( output_str == NULL )
			{
				perror("");
				exit(EXIT_FAILURE);
			}
			temp = strtok(NULL, " \n");
		}
		count++;
		output_str = realloc(output_str, (count + 1) * sizeof(char*));
		// Create an extra slot for null character.
		output_str[count] = NULL;
		if ( output_str == NULL )
		{
			perror("");
			exit(EXIT_FAILURE);
		}

		if ( strcmp(output_str[0], "cd") )
		{
			childPID = fork();
			if ( childPID >= 0 )
			{
				if ( childPID == 0 ) // Child process.
				{
					execvp(output_str[0], output_str);
					exit(EXIT_SUCCESS);
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
		// Clearance.
		int i;
		for ( i = 0; i < count; i++ )
		{
			output_str[i] = NULL;
		}
		print_prompt();
	}
	free(output_str);
	return 0;
}

void print_prompt(void)
{
	char time_buffer[BUFFER_SIZE];
	time_t raw_time;
	struct tm *timeinfo;

	if ( time(&raw_time) != -1 )
	{
		timeinfo = localtime(&raw_time);
		strftime(time_buffer, BUFFER_SIZE, TIME_FORMAT, timeinfo);
		printf(PS1, time_buffer);
	}
}
