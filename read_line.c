#include "shell.h"

/**
 * read_line - Read a line from stdin
 *
 * Return: Pointer to the line read
 */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            free(line);
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("read_line");
            free(line);
            exit(EXIT_FAILURE);
        }
    }
    return line;
}
