#include "shell.h"

/**
 * main - Entry point of the shell program
 *
 * Return: 0 on success
 */
int main(void)
{
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
