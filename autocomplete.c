/* autocomplete.c */

#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>

/* List of built-in commands */
char *builtin_commands[] = {
    "cd",
    "env",
    "help",
    "exit",
    "echo",
    NULL
};

/**
 * command_generator - Generator function for command autocompletion
 * @text: The text to match
 * @state: The state of the generator
 *
 * Return: Matched command or NULL
 */
char *command_generator(const char *text, int state)
{
    static int list_index, len;
    char *name;

    if (!state)
    {
        list_index = 0;
        len = strlen(text);
    }

    // First, check built-in commands
    while ((name = builtin_commands[list_index++]))
    {
        if (strncmp(name, text, len) == 0)
        {
            return strdup(name);
        }
    }

    // Then, check system commands in PATH
    static char **system_commands = NULL;
    static int system_index = 0;

    if (!system_commands)
    {
        system_commands = get_system_commands();
    }

    while (system_commands && system_commands[system_index])
    {
        name = system_commands[system_index++];
        if (strncmp(name, text, len) == 0)
        {
            return strdup(name);
        }
    }

    // Reset system commands for next completion
    if (!system_commands || !system_commands[system_index])
    {
        free_command_list(system_commands);
        system_commands = NULL;
        system_index = 0;
    }

    return NULL;
}

/**
 * simple_prompt_completion - Custom completion function
 * @text: The text to complete
 * @start: The start position
 * @end: The end position
 *
 * Return: Array of matches
 */
char **simple_prompt_completion(const char *text, int start, int end)
{
    char **matches = NULL;

    (void)end; // Unused parameter

    // If this is the first word, complete command names
    if (start == 0)
    {
        matches = rl_completion_matches(text, command_generator);
    }
    else
    {
        // Otherwise, complete file names
        rl_attempted_completion_over = 1;
        matches = rl_completion_matches(text, rl_filename_completion_function);
    }

    return matches;
}

/**
 * get_system_commands - Get a list of system commands from PATH
 *
 * Return: NULL-terminated array of command names
 */
char **get_system_commands(void)
{
    char *path = getenv("PATH");
    char *dir, *path_copy;
    char **command_list = NULL;
    size_t command_count = 0;
    struct dirent *entry;
    DIR *dp;

    if (!path)
        return NULL;

    path_copy = strdup(path);
    dir = strtok(path_copy, ":");

    while (dir)
    {
        dp = opendir(dir);
        if (dp)
        {
            while ((entry = readdir(dp)) != NULL)
            {
                if (entry->d_type == DT_REG || entry->d_type == DT_LNK || entry->d_type == DT_UNKNOWN)
                {
                    // Reallocate command list
                    command_list = realloc(command_list, sizeof(char *) * (command_count + 2));
                    command_list[command_count] = strdup(entry->d_name);
                    command_count++;
                }
            }
            closedir(dp);
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);

    if (command_list)
        command_list[command_count] = NULL;

    return command_list;
}

/**
 * free_command_list - Free the command list
 * @commands: The command list to free
 */
void free_command_list(char **commands)
{
    if (commands)
    {
        for (int i = 0; commands[i]; i++)
        {
            free(commands[i]);
        }
        free(commands);
    }
}
