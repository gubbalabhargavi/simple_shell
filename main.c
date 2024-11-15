/* main.c */

#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>

/* Function prototypes */
char *command_generator(const char *, int);
char **simple_prompt_completion(const char *, int, int);

/**
 * main - Entry point of the shell program
 *
 * Return: 0 on success
 */
int main(void)
{
    // Initialize Readline completion
    rl_attempted_completion_function = simple_prompt_completion;

    // Check if file descriptor is associated with a terminal
    if (isatty(STDIN_FILENO) == 1)
    {
        shell_interactive();
    }
    else
    {
        shell_no_interactive();
    }
    return (0);
}
