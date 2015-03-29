#include <stdio.h>
#include <stdlib.h>

void print_prompt(void);
char** parse_command(char* line);
void execute_command(char** output_str);

void shell_process(void)
{
	size_t len = 0u;
	ssize_t read;
	char* line = NULL;
	// Variables above are used in getline().
	char** arg_vectors;
	// The variable stores pointers to the beginning of each argument.

	print_prompt(); // Print the prompt for the first time.
	while ( (read = getline(&line, &len, stdin)) != -1 )
	// Check if there is EOF inputed from keyboard (Ctrl + d).
	{
		arg_vectors = parse_command(line);
		if ( arg_vectors == NULL )
		{
			print_prompt();
			continue;
		}
		else
		{
			execute_command(arg_vectors);
			print_prompt();
			free(arg_vectors); // Release the memory after each turn of use.
		}
	}
}
