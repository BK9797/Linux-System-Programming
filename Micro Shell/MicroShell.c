#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

void execute_command(char **args, char *input_file, char *output_file, char *error_file) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {  // Child process
        // Handle input redirection
        if (input_file) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Handle output redirection
        if (output_file) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Handle error redirection
        if (error_file) {
            int fd = open(error_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Error opening error file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        // Execute the command
        if (execvp(args[0], args) < 0) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        wait(NULL); // Parent waits for child process
    }
}

void parse_and_execute(char *input) {
    char *args[MAX_ARGS];
    char *input_file = NULL, *output_file = NULL, *error_file = NULL;
    int arg_count = 0;

    char *token = strtok(input, " ");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {  
            token = strtok(NULL, " ");
            if (token) input_file = token;
        } 
        else if (strcmp(token, ">") == 0) {  
            token = strtok(NULL, " ");
            if (token) output_file = token;
        } 
        else if (strcmp(token, "2>") == 0) {  
            token = strtok(NULL, " ");
            if (token) error_file = token;
        } 
        else {  
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (arg_count == 0) return;  

    // Handle built-in commands
    if (strcmp(args[0], "exit") == 0) {
        printf("Good Bye :)\n");
        exit(0);
    } 
    else if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_CMD_LEN];
        if (getcwd(cwd, sizeof(cwd))) {
            printf("%s\n", cwd);
        } else {
            perror("pwd failed");
        }
    } 
    else if (strcmp(args[0], "cd") == 0) {
        if (arg_count < 2) {
            fprintf(stderr, "cd: missing argument\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
        }
    } 
    else {
        execute_command(args, input_file, output_file, error_file);
    }
}

int main() {
    char input[MAX_CMD_LEN];

    while (1) {
        printf("Nano Shell > ");
        if (fgets(input, MAX_CMD_LEN, stdin) == NULL) {
            break;
        }
        
        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) > 0) {
            parse_and_execute(input);
        }
    }

    return 0;
}
