#include "shell.h"

char *read_stream(void)
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
            perror("read_stream");
            free(line);
            exit(EXIT_FAILURE);
        }
    }
    return line;
}
