# Simple Shell Project

## Overview

This project is a basic UNIX-style command-line interpreter (shell) written in C. It allows users to execute common shell commands and supports basic shell functionalities like command execution, piping, redirection, and background processes. This project serves as an introduction to the core concepts behind shell development.

## Features

- **Execute External Commands**: Run any command available in your system’s `$PATH`.
- **Built-in Commands**:
  - `cd`: Change the current directory.
  - `exit`: Exit the shell.
  - `help`: Display available commands.
  - `env`: Display environment variables.
- **Command Parsing with Quotes**: Supports single (`'`) and double (`"`) quotes.
- **Piping (`|`)**: Pass the output of one command as input to another.
- **Input and Output Redirection (`<`, `>`)**: Redirect input from and output to files.
- **Background Execution (`&`)**: Run commands in the background.
- **Error Handling**: Displays messages for unknown commands or syntax errors.

## Requirements

- **Operating System**: Linux or macOS (any UNIX-like environment)
- **Compiler**: GCC or any standard C compiler

## Installation

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/shahil3/simple_shell.git

2. **Navigate into the Project Directory**:

   ```bash
   cd simple_shell

3. **Compile the Project**:

   ```bash
   make

4. **Usage**:

    After compiling, run the shell by executing the simple_shell binary:

   ```bash
   ./simple_shell
   ```

    You will then see a prompt where you can enter commands, similar to other shells:
    ```bash
   simple_prompt$

# Examples
- **Basic Commands**: 
```bash
   simple_prompt$ ls
   simple_prompt$ pwd
   simple_prompt$ whoami
```
- **Commands with Arguments**: 
```bash
   simple_prompt$ grep "search_term" filename.txt
   simple_prompt$ cat file.txt

```
- **Piping and Redirection**: 
```bash
   simple_prompt$ cat file.txt | grep "pattern" > output.txt
   simple_prompt$ sort < unsorted.txt > sorted.txt

```
- **Background Processes:**: 
```bash
   simple_prompt$ sleep 30 &

```
- **Basic Commands**: 
```bash
   simple_prompt$ cd /path/to/directory
   simple_prompt$ help
   simple_prompt$ exit
```

## Project Structure

- **main.c**: Entry point for the shell program.
- **shell.h**: Header file containing function prototypes and data structures.
- **parse_line.c**: Contains the code for parsing commands and handling piping.
- **execute_command.c**: Handles the execution of commands, including redirection and piping.
- **builtins.c**: Implements built-in commands such as `cd`, `help`, and `exit`.
- **read_line.c**: Reads input from the user.
- **read_stream.c**: Reads input from non-interactive sources.
- **Makefile**: Contains rules for building the project.

## Contributing

Feel free to open issues or submit pull requests if you’d like to improve the functionality or fix any bugs. Contributions are always welcome!



## Author

*Shahil Sharma*

## Acknowledgments

- This project was part of an operating System Course work IITJ CSL3030.
- Thanks to all the resources and tutorials that provided guidance.
