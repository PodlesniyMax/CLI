#include <stdio.h>
#include <string.h>
#include "error_cli.h"

#define ERR_SIZE 100

static char err[ERR_SIZE];

void write_error(const char *str)
{
	strncpy(err, str, ERR_SIZE);
}

void print_error(void)
{
	printf("%s\n", err);
}
