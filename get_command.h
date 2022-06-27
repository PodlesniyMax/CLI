struct list {
	char *lexeme;
	struct list *next;
};

int get_command(struct list **lexeme_list);
