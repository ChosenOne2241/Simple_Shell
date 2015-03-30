#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>

void print_prompt(void);
void handler(int signo);

void set_handlers(void)
{
	// Set signal handlers for each situation.
  	signal(SIGWINCH, handler); // When the size of the window is changed.
	signal(SIGINT, handler); // Cope with Ctrl + c.
	signal(SIGQUIT, SIG_IGN); // Ignore Ctrl + \.
	signal(SIGTSTP, SIG_IGN); // Ignore Ctrl + z.
}

void handler(int signo)
{
	putchar('\n'); // End current line.
	switch ( signo )
	{
		case SIGWINCH:
			printf("Wow! You found a Easter egg!\n");
			printf("[SIGWINCH] signo = %d\n", signo);
			struct winsize ws;
			int rv = ioctl(0, TIOCGWINSZ, &ws);
			/*
			 * In fact, declaration of variables within switch { case ... } is
			 * quite improper according to the fact that C treats "case" as labels
			 * of a jump table rather than separate blocks of codes.
			 */
			printf("[IOCTL] rv = %d rows = %d cols = %d\n", rv, ws.ws_row, ws.ws_col);
			// There is no break here.
		case SIGINT:
			print_prompt();
			break;
		default:
			break;
	}
}
