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
    char cwd[200];
    char prompt[300];
    if (isatty(STDIN_FILENO))
    {
        getcwd(cwd, sizeof(cwd));  // Get current working directory
        snprintf(prompt, sizeof(prompt), "\033[1;32msimple-shell:\033[1;34m~%s\033[0m$ ", cwd); // Format the prompt string
        line = readline(prompt);

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
