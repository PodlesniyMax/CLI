#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "types_cli.h"
#include "get_command.h"
#include "process_command.h"
#include "error_cli.h"

void free_mem(struct list *lexeme_list);

int main(int argc, char **argv)
{
	int state = -1;
	struct list *lexeme_list = NULL;

	for (;;) {
		printf(" > ");
		state = get_command(&lexeme_list);
		if (state == end_of_file) {
			break;
		} else if (state == empty_str) {
			continue;
		} else if (state == error) {
			print_error();
			continue;
		}
		for (struct list *l = lexeme_list; l; l = l->next) {
			printf("[%s]\n", l->lexeme);
		}
		process_command(lexeme_list);
		free_mem(lexeme_list);
	}
	printf("\n");
	return 0;
}

void free_mem(struct list *lexeme_list)
{
	while (lexeme_list) {
		struct list *tmp = lexeme_list;
		lexeme_list = lexeme_list->next;
		free(tmp->lexeme);
		free(tmp);
	}
}
