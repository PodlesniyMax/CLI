struct list {
	char *lexeme;
	struct list *next;
};

enum {end_of_file = -1, success = 0, empty_str = 1, error = 2};

int get_command(struct list **lexeme_list);
