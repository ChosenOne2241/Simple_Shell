#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void print_prompt(void);
char** parse_command(char* command_line);
void execute_command(char** arg_vectors);

void shell_process(void)
{
	size_t buffer_length = 0u;
	char* command_line = NULL;
	// Variables above are used in getline().
	char** arg_vectors = NULL;
	// The variable stores pointers to the beginning of each argument.


	print_prompt(); // Print the prompt for the first time.
	while ( getline(&command_line, &buffer_length, stdin) != -1 )
	// Check if there is EOF inputed from keyboard (Ctrl + d).
	{
		arg_vectors = parse_command(command_line);
		if ( arg_vectors == NULL )
		{
			print_prompt();
			continue;
		}
		else
		{
			int fd, i, old_stdout;
			for ( i = 0; arg_vectors[i] != NULL; i++ )
			{
				if ( strcmp(arg_vectors[i], ">") == 0 )
				// Feature 5.
				{
					fd = open(arg_vectors[i + 1] , O_WRONLY | O_CREAT | O_TRUNC, 0666);
					arg_vectors[i] = NULL;
					old_stdout = dup(1);
					dup2(fd, 1);
					break;
				}
			}
			execute_command(arg_vectors);
			// Actually, we should close fd after we finish using it.
			dup2(old_stdout, 1);
		}
		print_prompt();
	}
	putchar('\n'); // End current line.
	free(command_line);
	free(arg_vectors);
}
