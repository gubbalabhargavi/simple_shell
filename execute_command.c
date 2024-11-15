/* execute_command.c */

#include "shell.h"

/**
 * execute_command - Executes commands, handling pipes, redirection,
 * and background processes.
 * @cmd: Pointer to the first command_t in the linked list.
 *
 * Return: 0 to exit the shell, 1 to continue.
 */
int execute_command(command_t *cmd)
{
    int fd_in = STDIN_FILENO;
    pid_t pid;
    int status = 1;
    int fd[2];

    command_t *current_cmd = cmd;

    while (current_cmd != NULL)
    {
        // Handle built-in commands in the parent process
        if (current_cmd->args[0] == NULL)
        {
            current_cmd = current_cmd->next;
            continue;
        }
        else if (strcmp(current_cmd->args[0], "cd") == 0 ||
                 strcmp(current_cmd->args[0], "exit") == 0 ||
                 strcmp(current_cmd->args[0], "help") == 0 ||
                 strcmp(current_cmd->args[0], "env") == 0 ||
                 strcmp(current_cmd->args[0], "echo") == 0)
        {
            // Save original file descriptors
            int saved_stdin = dup(STDIN_FILENO);
            int saved_stdout = dup(STDOUT_FILENO);

            // Input redirection
            if (current_cmd->input_redirect)
            {
                int fd0 = open(current_cmd->input_redirect, O_RDONLY);
                if (fd0 < 0)
                {
                    perror("Input redirection");
                    // Restore original file descriptors
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin);
                    close(saved_stdout);
                    current_cmd = current_cmd->next;
                    continue;
                }
                dup2(fd0, STDIN_FILENO);
                close(fd0);
            }

            // Output redirection
            if (current_cmd->output_redirect)
            {
                int fd1 = open(current_cmd->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd1 < 0)
                {
                    perror("Output redirection");
                    // Restore original file descriptors
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin);
                    close(saved_stdout);
                    current_cmd = current_cmd->next;
                    continue;
                }
                dup2(fd1, STDOUT_FILENO);
                close(fd1);
            }

            // Execute the built-in command
            if (strcmp(current_cmd->args[0], "cd") == 0)
            {
                own_cd(current_cmd->args);
            }
            else if (strcmp(current_cmd->args[0], "exit") == 0)
            {
                own_exit(current_cmd->args);
                // Restore original file descriptors before exiting
                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                return 0;  // Exit the shell
            }
            else if (strcmp(current_cmd->args[0], "help") == 0)
            {
                own_help(current_cmd->args);
            }
            else if (strcmp(current_cmd->args[0], "env") == 0)
            {
                own_env(current_cmd->args);
            }
            else if (strcmp(current_cmd->args[0], "echo") == 0)
            {
                own_echo(current_cmd->args);
            }

            // Restore original file descriptors
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);

            current_cmd = current_cmd->next;
            continue;
        }

        // Create a pipe if there's a next command
        if (current_cmd->next != NULL)
        {
            if (pipe(fd) < 0)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == 0)
        {
            // Child process

            // Input redirection
            if (current_cmd->input_redirect)
            {
                fd_in = open(current_cmd->input_redirect, O_RDONLY);
                if (fd_in < 0)
                {
                    perror("Input redirection");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            else if (fd_in != STDIN_FILENO)
            {
                dup2(fd_in, STDIN_FILENO);
            }

            // Output redirection
            if (current_cmd->output_redirect)
            {
                int fd_out = open(current_cmd->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out < 0)
                {
                    perror("Output redirection");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
            else if (current_cmd->next != NULL)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }

            // Close unused file descriptors
            if (current_cmd->next != NULL)
                close(fd[0]);

            // Execute the command
            if (execvp(current_cmd->args[0], current_cmd->args) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid < 0)
        {
            // Error forking
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process
            if (!current_cmd->background)
            {
                waitpid(pid, NULL, 0);
            }

            // Close file descriptors
            if (fd_in != STDIN_FILENO)
                close(fd_in);
            if (current_cmd->next != NULL)
                close(fd[1]);

            // Save the input for the next command
            fd_in = (current_cmd->next != NULL) ? fd[0] : STDIN_FILENO;
        }

        current_cmd = current_cmd->next;
    }

    return status;
}

// void cleanup_command_execution(int *fd, pid_t pid, int *status)
// {
//     if (fd[0] != -1) close(fd[0]);
//     if (fd[1] != -1) close(fd[1]);
//     if (pid > 0) waitpid(pid, status, 0);
// }
