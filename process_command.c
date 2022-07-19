#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "types_cli.h"
#include "process_command.h"
#include "error_cli.h"

typedef struct {
	char **vector;
	size_t size;
	size_t capacity;
} Vector;

static int init_vector(Vector *v);
static int insert_vector(Vector *v, char *s);
static void free_vector(Vector *v);

int process_command(struct list *lexeme_list)
{
	Vector v;
	pid_t pid;
	int state;

	if (init_vector(&v) == -1) {
		goto end;
	}

	for (; lexeme_list; lexeme_list = lexeme_list->next) {
		state = insert_vector(&v, lexeme_list->lexeme);
		if (state == -1) {
			goto end;
		}
	}
	state = insert_vector(&v, NULL);
	if (state == -1) {
		goto end;
	}

	if (strcmp(v.vector[0], "cd") == 0) {
		if (v.size > 3) { /* cd path NULL */
			fprintf(stderr, "Too many arrguments\n");
			fprintf(stderr, "Usage: cd [path]\n");
			goto end;
		}
		if (v.size == 2) {
			state = chdir(getenv("HOME"));
			if (state == -1) {
				perror("chdir");
			}
			goto end;
		}
		state = chdir(v.vector[1]);
		if (state == -1) {
			perror("chdir");
		}
		goto end;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		goto end;
	}
	if (pid == 0) {
		execvp(v.vector[0], v.vector);
		perror(v.vector[0]);
		exit(EXIT_FAILURE);
	}
	wait(NULL);

	end:
	free_vector(&v);

	return 0;
}

static int init_vector(Vector *v)
{
	v->capacity = 8;
	v->size = 0;
	v->vector = (char **)malloc(sizeof(char *) * v->capacity);
	if (!v->vector) {
		perror("init_vector");
		return -1;
	}

	return 0;
}

static int insert_vector(Vector *v, char *s)
{
	if (!v->vector) {
		init_vector(v);
	}
	if (v->size >= v->capacity) {
		v->capacity *= 2;
		v->vector = realloc(v->vector, v->capacity);
		if (!v->vector) {
			perror("insert_vector");
			return -1;
		}
	}
	v->vector[v->size] = s;
	v->size++;

	return 0;
}

static void free_vector(Vector *v)
{
	free(v->vector);
	v->vector = NULL;
	v->capacity = 0;
	v->size = 0;
}
