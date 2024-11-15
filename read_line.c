/* read_line.c */

#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>

/**
 * read_line - Read a line from stdin, using readline if interactive
 *
 * Return: Pointer to the line read
 */
char *read_line(void)
{
    char *line = NULL;

    if (isatty(STDIN_FILENO))
    {
        line = readline("simple_prompt$ ");

        if (line && *line)
        {
            add_history(line);
        }
    }
    else
    {
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
    }

    return line;
}
