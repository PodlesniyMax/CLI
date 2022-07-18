#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "get_command.h"
#include "error_cli.h"

static int push_input_to_buffer(char **buff);
static bool is_correct_str(const char *str);
static struct list *split_str_on_lexemes_list(const char *str);
static char *get_next_lexeme(const char **str);
static bool is_buff_big_enough(int buff_size, int lexeme_size);
static int add_lexeme_to_list(struct list **lexemes_list, char *lexeme);
static bool is_empty(const char *str);

int get_command(struct list **lexemes_list)
{
	int state;
	char *buff = NULL;

	state = push_input_to_buffer(&buff);
	if (state == end_of_file) {
		return end_of_file;
	}
	if (is_empty(buff)) {
		return empty_str;
	}
	if (!is_correct_str(buff)) {
		return error;
	}
	*lexemes_list = split_str_on_lexemes_list(buff);
	free(buff);
	return success;
}

static int push_input_to_buffer(char **buff)
{
	int size = 5;
	int c_count = 0;
	char c;

	*buff = malloc(sizeof(char) * size);
	if (!*buff) {
		printf("Error: fail memmory allocation.");
		return end_of_file;
	}

	while ((c = getchar()) != EOF) {
		if (c_count >= size - 1) {
			size *= 2;
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

	if (c == EOF) {
		free(*buff);
		return end_of_file;
	}

	return c_count;
}

static bool is_correct_str(const char *str)
{
	int counter = 0;

	for (; *str; str++) {
		if (*str == '"') {
			counter++;
		}
		if (*str == '\\') {
			str++;
			if (*str != '\\' && *str != '"') {
				write_error("Incorrect escape sequence");
				return false;
			}
		}
	}
	if (counter % 2 != 0) {
		write_error("Incorrect number of \" simbols");
		return false;
	}

	return true;
}

static bool is_empty(const char *str)
{
	while (*str == ' ' || *str == '\t') {
		str++;
	}
	return *str == '\n' ? true : false;
}

static struct list *split_str_on_lexemes_list(const char *str)
{
	struct list *lexemes_list = NULL;
	char *lexeme = NULL;

	while ((lexeme = get_next_lexeme(&str))) {
		add_lexeme_to_list(&lexemes_list, lexeme);
	}
	return lexemes_list;
}

static char *get_next_lexeme(const char **str)
{
	enum { out_quote, in_quote };
	const char *s = *str;
	char *lexeme = NULL;
	int buff_size = 5, pos = 0, state = out_quote;

	if (is_empty(s)) {
		return NULL;
	}

	lexeme = malloc(sizeof(char) * buff_size);
	if (lexeme == NULL) {
		return NULL;
	}
	while (*s == ' ' || *s == '\t') {
		s++;
	}
	for (; *s != '\n'; s++) {
		if (state == out_quote && (*s == ' ' || *s == '\t')) {
			break;
		}
		if (*s == '"') {
			state = (state == out_quote ? in_quote : out_quote);
			continue;
		}
		if (*s == '\\') {
			s++;
		}
		if (!is_buff_big_enough(buff_size, pos + 1)) {
			buff_size *= 2;
			lexeme = realloc(lexeme, buff_size);
		}
		lexeme[pos] = *s;
		pos++;
	}
	lexeme[pos] = '\0';
	*str = s;
	return lexeme;
}

static bool is_buff_big_enough(int buff_size, int lexem_size)
{
	return buff_size >= lexem_size + 1 ? true : false;
}

static int add_lexeme_to_list(struct list **lexemes_list, char *lexeme)
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
