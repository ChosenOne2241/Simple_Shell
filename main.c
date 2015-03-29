void set_handlers(void);
void shell_process(void);

int main(void)
{
	set_handlers();
	shell_process();
	return 0;
}
