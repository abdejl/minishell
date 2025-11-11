Minishell (42 School Project)

Overview

Minishell is a 42 school project that involves creating a minimalist command-line interpreter, similar to `bash`. The goal is to build a program that can read a command from the standard input, parse it, and execute it, while handling a wide range of shell functionalities.

* This project provided a comprehensive understanding of:
* Process Management: Creating and managing processes (`fork`, `waitpid`).
* Command Execution: Executing system binaries (`execve`).
* Inter-Process Communication: Implementing pipes (`|`) for command chaining.
* File Descriptors: Managing I/O with file descriptors and implementing redirections (`<`, `>`, `>>`, `<<`).
* Signal Handling: Intercepting and handling signals like `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.
* Parsing: Building a robust lexer and parser to interpret the command line.
* Environment Variables: Handling and expanding environment variables (`$VAR`, `$?`).

Core Features

My Minishell successfully implements the following features:

* Interactive Prompt: Displays a prompt and waits for user input.
* Command History: Manages command history (navigable with up/down arrows), powered by the `readline` library.
* Command Execution: Finds and executes system binaries from the `PATH (e.g., `ls`, `grep`, `awk`).
* Built-in Commands:
    * `echo` (with the `-n` option)
    * `cd` (changing the current directory)
    * `pwd` (displaying the current directory)
    * `export` (for managing environment variables)
    * `unset` (for removing environment variables)
    * `env` (displaying the environment)
    * `exit` (exiting the shell)
* Pipes (`|`): Full support for single and multiple pipes, allowing the output of one command to be chained as the input to the next.
    * `e.g., ls -l | grep ".c" | wc -l`
* Redirections:
    * `>`: Redirect output to a file.
    * `<`: Redirect input from a file.
    * `>>`: Append output to a file.
* Heredoc (`<<`): Here Document support for multi-line input.
* Environment Variable Expansion: Correctly expands variables like `$USER`, `$HOME`, or any custom-set variable. Also supports `$?` to get the exit status of the last command.
* Signal Handling:
    * `Ctrl-C`: Displays a new prompt on a new line.
    * `Ctrl-D`: Exits the shell.
    * `Ctrl-\`: Does nothing.

How It Works: The Execution Pipeline

The shell processes input in several distinct stages:

1. Read: Reads a line of input from the user using `readline`.
2. Lexer (Tokenizer): The raw input string is broken down into a list of tokens (e.g., `TOKEN_CMD`, `TOKEN_PIPE`, `TOKEN_REDIRECT_OUT`, `TOKEN_ARG`).
3. Parser: The list of tokens is organized into a structured data format (like an Abstract Syntax Tree or a list of command structures), which respects command precedence and relationships.
4. Expander: The parser's output is traversed to expand environment variables (`$VAR`) and handle quotes (`'` and `"`).
5. Executor: This final stage walks the command structure and executes it.
    * It sets up file descriptors for all pipes and redirections.
    * It `fork()`-s a new child process for each command.
    * The child process executes the command (either a built-in function or an external binary via `execve`).
    * The parent process waits for all child processes to complete.

Getting Started

Prerequisites

This project requires the `readline` library.

* On macOS (with Homebrew):
    ```
    brew install readline
    ```
* On Debian/Ubuntu:
    ```
    sudo apt-get install libreadline-dev
    ```

Installation and Testing

1.  Clone the repository:
    ```
    git clone [https://github.com/abdejl/minishell.git](https://github.com/abdejl/minishell.git)
    ```

2.  Navigate to the project directory:
    ```
    cd minishell
    ```

3.  Compile the project:
    ```
    make
    ```

4.  Run the shell:
    ```
    ./minishell
    ```

Example

Once running, you can test the shell with various commands.

```
Welcome to Minishell!
minishell> ls -l | grep "Makefile" > output.txt
minishell> cat output.txt
-rw-r--r--  1 abdejl  staff  1234 Nov 11 10:20 Makefile
minishell> echo "Hello $USER"
Hello abdejl
minishell> exit
