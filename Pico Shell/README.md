# Pico Shell

A simple shell written in C with support for built-in and external commands.

## Features:
- **Built-in Commands:**
  - `echo <text>` → Prints the text.
  - `pwd` → Prints the current working directory.
  - `cd <dir>` → Changes the directory.
  - `exit` → Exits the shell.

- **External Commands:**  
  - Any other command (e.g., `ls`, `grep`, `gcc`, etc.) runs using `fork + execvp`.

- **Command Parsing:**
  - Splits user input into arguments, handling multiple spaces correctly.

- **Memory Management:**
  - Uses **dynamic allocation** for argument parsing to ensure flexibility.

## Compilation:
```sh
gcc PicoShell.c -o PicoShell
