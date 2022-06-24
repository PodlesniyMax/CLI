#include <stdio.h>
#include <stdlib.h>

void get_command(void);
void process_command(void);
void free_mem(void);
int push_input_to_buffer(char **buff, int *buff_size);

int main(int argc, char **argv)
{
	int state = 0;
	int buff_size = 0;
	char *buff = NULL;

	for (;;) {
		state = push_input_to_buffer(&buff, &buff_size);
		printf("state = %d\n", state);
		if (state <= 0)
			break;
		printf("%s", buff);
		get_command();
		process_command();
		free_mem();
		free(buff);
	}
	return 0;
}

int push_input_to_buffer(char **buff, int *buff_size)
{
	int size = 5;
	int c_count = 0;
	char c;

	*buff_size = size;

	*buff = malloc(sizeof(char) * *buff_size);
	if (!*buff)
		return 0;

	while ((c = getchar()) != EOF) {
		if (c_count >= *buff_size - 1) {
			*buff_size += size;
			printf("Realloc memmory, new buff size = %d\n", *buff_size);
			*buff = realloc(*buff, *buff_size);
			if (!*buff)
				return 0;
		}

		(*buff)[c_count] = c;
		c_count++;

		if (c == '\n')
			break;
	}
	(*buff)[c_count] = '\0';

	if (c == EOF)
		return -1;

	return c_count;
}
void get_command(void){}
void process_command(void){}
void free_mem(void){}
