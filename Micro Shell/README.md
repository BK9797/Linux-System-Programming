# Micro Shell

Nano Shell is a simple command-line shell written in C. It supports basic built-in commands, executes external commands, and handles input/output redirections.

##  Features
- Built-in commands: `echo`, `pwd`, `cd`, `exit`  
- External command execution using `execvp()`  
- Input/output redirection (`<`, `>`, `2>`)  
- Proper error handling for invalid commands and file access  
- Dynamic argument parsing  

##  Installation & Compilation
To compile Micro Shell, run:

```sh
gcc MicroShell.c -o MicroShell
