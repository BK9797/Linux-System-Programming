#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

// Function to parse the command line input into arguments
char **parse_input(char *input, int *arg_count) {
    char **args = NULL;
    char *token = strtok(input, " "); 
    int count = 0;

    while (token != NULL) {
        args = realloc(args, sizeof(char *) * (count + 1));
        if (!args) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        args[count++] = strdup(token);
        token = strtok(NULL, " ");
    }

    // NULL terminate the arguments list
    args = realloc(args, sizeof(char *) * (count + 1));
    args[count] = NULL;
    *arg_count = count;

    return args;
}

// Function to execute built-in commands (echo, pwd, cd, exit)
int execute_builtin(char **args, int arg_count) {
    if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; i < arg_count; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
        return 1;
    } 
    else if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_INPUT];
        if (getcwd(cwd, sizeof(cwd))) {
            printf("%s\n", cwd);
        } else {
            perror("pwd failed");
        }
        return 1;
    } 
    else if (strcmp(args[0], "cd") == 0) {
        if (arg_count < 2) {
            fprintf(stderr, "cd: missing argument\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    } 
    else if (strcmp(args[0], "exit") == 0) {
        printf("Good Bye :)\n");
        exit(0);
    }
    
    return 0; // Not a built-in command
}

// Function to execute external commands
void execute_external(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) { 
        execvp(args[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    } 
    else { 
        wait(NULL); // Wait for the child process
    }
}

// Function to free dynamically allocated memory
void free_args(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
}

// Main shell loop
int main() {
    char input[MAX_INPUT];

    while (1) {
        printf("Pico Shell >>> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue; // Ignore empty input

        int arg_count;
        char **args = parse_input(input, &arg_count);

        if (args[0] != NULL) {
            if (!execute_builtin(args, arg_count)) {
                execute_external(args);
            }
        }

        free_args(args); // Clean up memory
    }

    return 0;
}
