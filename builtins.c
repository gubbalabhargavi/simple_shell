#include "shell.h"

int own_env(char **args)
{
    (void)args;  // Suppress the unused parameter warning

    extern char **environ;
    for (char **env = environ; *env != 0; env++)
    {
        printf("%s\n", *env);
    }
    return 0;
}

int own_help(char **args)
{
    (void)args;  // Suppress the unused parameter warning

    printf("Simple Shell Help\n");
    printf("Built-in commands:\n");
    printf("  cd\n");
    printf("  env\n");
    printf("  help\n");
    printf("  exit\n");
    printf("Type 'help [command]' for more information on a specific command.\n");
    return 0;
}

int own_exit(char **args)
{
    (void)args;  // Suppress the unused parameter warning

    exit(0);
}
int own_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
            perror("cd");
    }
    return 0;
}

