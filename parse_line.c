#include "shell.h"

/**
 * get_token - Extracts the next token from the input string,
 * handling quotes.
 * @str_ptr: Pointer to the input string pointer.
 *
 * Return: The extracted token.
 */
char *get_token(char **str_ptr)
{
    char *s = *str_ptr;
    size_t bufsize = TOK_BUFSIZE;
    char *token = malloc(bufsize);
    int position = 0;
    int in_quotes = 0;
    char quote_char = '\0';

    if (!token)
    {
        fprintf(stderr, "Allocation error in get_token\n");
        exit(EXIT_FAILURE);
    }

    while (*s != '\0' && *s != '\n')
    {
        if (in_quotes)
        {
            if (*s == quote_char)
            {
                in_quotes = 0;
                s++;
            }
            else
            {
                token[position++] = *s++;
            }
        }
        else
        {
            if (*s == '\'' || *s == '"')
            {
                in_quotes = 1;
                quote_char = *s;
                s++;
            }
            else if (*s == ' ' || *s == '\t')
            {
                s++;
                break;
            }
            else
            {
                token[position++] = *s++;
            }
        }

        // Reallocate if necessary
        if (position >= (int)bufsize)
        {
            bufsize += TOK_BUFSIZE;
            token = realloc(token, bufsize);
            if (!token)
            {
                fprintf(stderr, "Reallocation error in get_token\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    token[position] = '\0';
    *str_ptr = s;
    return token;
}

/**
 * parse_line - Parses the input line into commands, handling pipes,
 * redirections, background processes, and quotes.
 * @line: The input line.
 *
 * Return: Pointer to the first command_t in the linked list.
 */
command_t *parse_line(char *line)
{
    command_t *head = NULL, *current = NULL;
    char *segment, *saveptr;

    segment = strtok_r(line, "|", &saveptr);
    while (segment != NULL)
    {
        command_t *cmd = malloc(sizeof(command_t));
        if (!cmd)
        {
            fprintf(stderr, "Allocation error in parse_line\n");
            exit(EXIT_FAILURE);
        }
        cmd->args = malloc(TOK_BUFSIZE * sizeof(char *));
        if (!cmd->args)
        {
            fprintf(stderr, "Allocation error in parse_line\n");
            exit(EXIT_FAILURE);
        }
        cmd->input_redirect = NULL;
        cmd->output_redirect = NULL;
        cmd->background = 0;
        cmd->next = NULL;

        int position = 0;
        char *s = segment;
        while (*s != '\0')
        {
            // Skip whitespace and newlines
            while (*s == ' ' || *s == '\t' || *s == '\n')
                s++;

            if (*s == '\0')
                break;

            // Handle input redirection
            if (*s == '<')
            {
                s++;
                while (*s == ' ' || *s == '\t')
                    s++;
                cmd->input_redirect = get_token(&s);
            }
            // Handle output redirection
            else if (*s == '>')
            {
                s++;
                while (*s == ' ' || *s == '\t')
                    s++;
                cmd->output_redirect = get_token(&s);
            }
            // Handle background process
            else if (*s == '&')
            {
                cmd->background = 1;
                s++;
            }
            else
            {
                char *token = get_token(&s);
                if (token[0] != '\0') // Only add non-empty tokens
                {
                    cmd->args[position++] = token;

                    // Reallocate if necessary
                    if (position >= TOK_BUFSIZE)
                    {
                        cmd->args = realloc(cmd->args, (position + TOK_BUFSIZE) * sizeof(char *));
                        if (!cmd->args)
                        {
                            fprintf(stderr, "Reallocation error in parse_line\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else
                {
                    free(token);
                }
            }
        }
        cmd->args[position] = NULL;

        if (!head)
            head = cmd;
        else
            current->next = cmd;
        current = cmd;

        segment = strtok_r(NULL, "|", &saveptr);
    }
    return head;
}
