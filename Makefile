CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SOURCES = main.c shell_interactive.c shell_no_interactive.c read_line.c read_stream.c parse_line.c execute_command.c builtins.c
HEADERS = shell.h
OBJECTS = $(SOURCES:.c=.o)
TARGET = simple_shell

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	