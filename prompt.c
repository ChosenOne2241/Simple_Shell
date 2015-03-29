#include <time.h>
#include <stdio.h>

#define PS1 "[%s] # "
#define TIME_FORMAT "%d/%m %R"
#define BUFFER_SIZE 40

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
