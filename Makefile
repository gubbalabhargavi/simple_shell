CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
LIBS = -lreadline

all: simple_shell

simple_shell: main.o shell_interactive.o shell_no_interactive.o \
              read_line.o parse_line.o execute_command.o builtins.o autocomplete.o
	$(CC) $(CFLAGS) -o simple_shell main.o shell_interactive.o \
	shell_no_interactive.o read_line.o parse_line.o execute_command.o \
	builtins.o autocomplete.o $(LIBS)

main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c

shell_interactive.o: shell_interactive.c shell.h
	$(CC) $(CFLAGS) -c shell_interactive.c

shell_no_interactive.o: shell_no_interactive.c shell.h
	$(CC) $(CFLAGS) -c shell_no_interactive.c

read_line.o: read_line.c shell.h
	$(CC) $(CFLAGS) -c read_line.c

parse_line.o: parse_line.c shell.h
	$(CC) $(CFLAGS) -c parse_line.c

execute_command.o: execute_command.c shell.h
	$(CC) $(CFLAGS) -c execute_command.c

builtins.o: builtins.c shell.h
	$(CC) $(CFLAGS) -c builtins.c

autocomplete.o: autocomplete.c shell.h
	$(CC) $(CFLAGS) -c autocomplete.c

clean:
	rm -f *.o simple_shell
