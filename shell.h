#ifndef SHELL_H
#define SHELL_H

/*---LIBRARIES---*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

/*---DEFINITIONS---*/
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

/*---DATA STRUCTURES---*/
typedef struct command_t {
    char **args;                  // Command and arguments
    char *input_redirect;         // Input redirection file
    char *output_redirect;        // Output redirection file
    int background;               // Background process flag
    struct command_t *next;       // Next command in the pipeline
} command_t;

/*---FUNCTION PROTOTYPES---*/
/* parse_line.c */
command_t *parse_line(char *line);
char *get_token(char **str_ptr);

/* main.c */
void shell_interactive(void);
void shell_no_interactive(void);

/* read_line.c */
char *read_line(void);
char *read_stream(void);

/* execute_command.c */
int execute_command(command_t *cmd);

/* built-in commands */
int own_cd(char **args);
int own_env(char **args);
int own_help(char **args);
int own_exit(char **args);

#endif
