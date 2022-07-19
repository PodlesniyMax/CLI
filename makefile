objects = main.o get_command.o error_cli.o process_command.o

prog: $(objects)
	cc -g -Wall -o prog $(objects)

main.o: main.c get_command.h process_command.h error_cli.h types_cli.h
	cc -g -Wall -c main.c
get_command.o: get_command.c get_command.h error_cli.h types_cli.h
	cc -g -Wall -c get_command.c
error_cli.o: error_cli.c error_cli.h
	cc -g -Wall -c error_cli.c
process_command.o: process_command.c process_command.h error_cli.h \
	types_cli.h
	cc -g -Wall -c process_command.c

clean:
	rm prog $(objects)
