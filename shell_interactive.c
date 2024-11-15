#include "shell.h"

/**
 * shell_interactive - Runs the shell in interactive mode
 */
void shell_interactive(void)
{
    char *line;
    command_t *cmd_list;
    int status = 1;

    do {
        // printf("simple_prompt$ ");
        line = read_line();

        if (!line)
        {
            printf("\n");
            break;
        }

        cmd_list = parse_line(line);
        status = execute_command(cmd_list);

        // Free the command list
        command_t *tmp;
        while (cmd_list)
        {
            tmp = cmd_list;
            cmd_list = cmd_list->next;

            // Free each argument
            for (int i = 0; tmp->args[i]; i++)
                free(tmp->args[i]);
            free(tmp->args);

            if (tmp->input_redirect)
                free(tmp->input_redirect);
            if (tmp->output_redirect)
                free(tmp->output_redirect);
            free(tmp);
        }

        free(line);

    } while (status);
}
