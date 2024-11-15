/* builtins.c */

#include "shell.h"

/* Existing built-in functions */

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
    printf("  echo\n");  // Added echo to the help menu
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

/* Added own_echo function */

int own_echo(char **args)
{
    int i = 1;
    int newline = 1;
    int interpret_escapes = 0;

    // Check for options
    if (args[i] != NULL && args[i][0] == '-')
    {
        for (int j = 1; args[i][j] != '\0'; j++)
        {
            if (args[i][j] == 'n')
                newline = 0;
            else if (args[i][j] == 'e')
                interpret_escapes = 1;
            else if (args[i][j] == 'E')
                interpret_escapes = 0;
            else
            {
                fprintf(stderr, "echo: invalid option -- '%c'\n", args[i][j]);
                return 1;
            }
        }
        i++;
    }

    // Print the arguments
    for (; args[i] != NULL; i++)
    {
        if (interpret_escapes)
        {
            // Process escape sequences
            char *str = args[i];
            while (*str)
            {
                if (*str == '\\')
                {
                    str++;
                    switch (*str)
                    {
                        case 'a':
                            putchar('\a');
                            break;
                        case 'b':
                            putchar('\b');
                            break;
                        case 'c':
                            // Stop processing
                            return 0;
                        case 'e':
                            putchar('\033');  // Replaced '\e' with '\033'
                            break;
                        case 'f':
                            putchar('\f');
                            break;
                        case 'n':
                            putchar('\n');
                            break;
                        case 'r':
                            putchar('\r');
                            break;
                        case 't':
                            putchar('\t');
                            break;
                        case 'v':
                            putchar('\v');
                            break;
                        case '\\':
                            putchar('\\');
                            break;
                        case '\'':
                            putchar('\'');
                            break;
                        case '\"':
                            putchar('\"');
                            break;
                        case '?':
                            putchar('\?');
                            break;
                        case '0':
                            // Octal value
                            {
                                int val = 0;
                                int count = 0;
                                while (*str >= '0' && *str <= '7' && count < 3)
                                {
                                    val = val * 8 + (*str - '0');
                                    str++;
                                    count++;
                                }
                                putchar(val);
                                str--; // Adjust because of the extra increment in the while loop
                            }
                            break;
                        default:
                            putchar('\\');
                            putchar(*str);
                            break;
                    }
                }
                else
                {
                    putchar(*str);
                }
                str++;
            }
        }
        else
        {
            printf("%s", args[i]);
        }

        if (args[i + 1] != NULL)
            putchar(' ');
    }

    if (newline)
        putchar('\n');

    return 0;
}
