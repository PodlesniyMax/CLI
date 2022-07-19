#ifndef GET_COMMAND_T

#define GET_COMMAND_T

enum {end_of_file = -1, success = 0, empty_str = 1, error = 2};

int get_command(struct list **lexeme_list);

#endif
