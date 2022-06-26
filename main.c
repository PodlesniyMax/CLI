#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct list {
	char *lexeme;
	struct list *next;
};

int get_command(struct list **lexeme_list);
void process_command(void);
void free_mem(void);
int push_input_to_buffer(char **buff);
bool is_correct_str(const char *str);
struct list *split_str_on_lexemes_list(const char *str);
char *get_next_lexeme(const char **str);
bool is_buff_big_enough(int buff_size, int lexem_size);
int add_lexeme_to_list(struct list **lexemes_list, char *lexeme);
bool is_empty(const char *str);

int main(int argc, char **argv)
{
	int state = -1;
	struct list *lexeme_list = NULL;

	for (;;) {
		state = get_command(&lexeme_list);
		if (state != 0) {
			break;
		}
		for (struct list *l = lexeme_list; l; l = l->next) {
			printf("%s\n", l->lexeme);
		}
		process_command();
		free_mem();
	}
	return 0;
}

int push_input_to_buffer(char **buff)
{
	int size = 5;
	int c_count = 0;
	char c;

	*buff = malloc(sizeof(char) * size);
	if (!*buff)
		return 0;

	while ((c = getchar()) != EOF) {
		if (c_count >= size - 1) {
			size *= 2;
			printf("Realloc memmory, new buff size = %d\n", size);
			*buff = realloc(*buff, size);
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

bool is_correct_str(const char *str)
{
	int counter = 0;

	for (; *str; str++) {
		if (*str == '"') {
			counter++;
		}
	}

	return counter % 2 == 0 ? true : false;
}

bool is_empty(const char *str)
{
	while (*str == ' ' || *str == '\t') {
		str++;
	}
	return *str == '\n' ? true : false;
}

struct list *split_str_on_lexemes_list(const char *str)
{
	struct list *lexemes_list = NULL;
	char *lexeme = NULL;

	while ((lexeme = get_next_lexeme(&str))) {
		add_lexeme_to_list(&lexemes_list, lexeme);
	}
	return lexemes_list;
}

char *get_next_lexeme(const char **str)
{
	const char *s = *str;
	char *lexeme = NULL;
	int buff_size = 5, pos = 0;

	lexeme = malloc(sizeof(char) * buff_size);
	if (lexeme == NULL) {
		return NULL;
	}
	while (*s == ' ' || *s == '\t') {
		s++;
	}
	while (*s != ' ' && *s != '\t' && *s != '\n') {
		if (!is_buff_big_enough(buff_size, pos + 1)) {
			buff_size *= 2;
			lexeme = realloc(lexeme, buff_size);
		}
		lexeme[pos] = *s;
		pos++;
		s++;
	}
	lexeme[pos] = '\0';
	*str = s;
	if (pos == 0) {
		free(lexeme);
		lexeme = NULL;
	}
	return lexeme;
}

bool is_buff_big_enough(int buff_size, int lexem_size)
{
	return buff_size >= lexem_size + 1 ? true : false;
}

int add_lexeme_to_list(struct list **lexemes_list, char *lexeme)
{
	enum { fail = 0, success = 1 };
	struct list *node = NULL, *p;

	node = malloc(sizeof(struct list));
	if (node == NULL) {
		return fail;
	}
	node->lexeme = lexeme;
	node->next = NULL;
	if (*lexemes_list == NULL) {
		*lexemes_list = node;
	} else {
		for (p = *lexemes_list; p->next; p = p->next) {
		}
		p->next = node;
	}
	return success;
}

int get_command(struct list **lexemes_list)
{
	int state = 0;
	char *buff = NULL;

	state = push_input_to_buffer(&buff);
	if (state <= 0) {
		return -1;
	}
	if (is_empty(buff)) {
		return 1;
	}
	if (!is_correct_str(buff)) {
		return 2;
	}
	*lexemes_list = split_str_on_lexemes_list(buff);
	free(buff);
	return 0;
}
void process_command(void){}
void free_mem(void){}
