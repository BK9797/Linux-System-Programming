#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

// Structure for storing shell variables
typedef struct {
    char *name;
    char *value;
} ShellVar;

ShellVar *variables = NULL;
int var_count = 0;

// Function to set a shell variable
void set_variable(const char *name, const char *value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            free(variables[i].value);
            variables[i].value = strdup(value);
            return;
        }
    }

    variables = realloc(variables, sizeof(ShellVar) * (var_count + 1));
    variables[var_count].name = strdup(name);
    variables[var_count].value = strdup(value);
    var_count++;
}

// Function to get a shell variable
char *get_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    return "";
}

// Function to export a shell variable to the environment
void export_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            setenv(name, variables[i].value, 1);
            return;
        }
    }
    fprintf(stderr, "export: variable not found\n");
}

// Function to parse input into arguments
char **parse_input(char *input, int *arg_count) {
    char **args = NULL;
    char *token = strtok(input, " ");
    int count = 0;

    while (token != NULL) {
        args = realloc(args, sizeof(char *) * (count + 1));
        args[count++] = strdup(token);
        token = strtok(NULL, " ");
    }

    args = realloc(args, sizeof(char *) * (count + 1));
    args[count] = NULL;
    *arg_count = count;

    return args;
}

// Function to substitute variables in the command line
void substitute_variables(char **args, int arg_count) {
    for (int i = 0; i < arg_count; i++) {
        if (args[i][0] == '$') {
            char *value = get_variable(args[i] + 1);
            free(args[i]);
            args[i] = strdup(value);
        }
    }
}

// Function to execute built-in commands
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
    else if (strcmp(args[0], "export") == 0) {
        if (arg_count < 2) {
            fprintf(stderr, "export: missing variable name\n");
        } else {
            export_variable(args[1]);
        }
        return 1;
    }

    return 0;
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
        wait(NULL);
    }
}

// Function to check if the command is a variable assignment
int is_variable_assignment(char *input) {
    if (strchr(input, '=') == NULL) return 0;
    
    char *equal_pos = strchr(input, '=');
    if (equal_pos == input || *(equal_pos + 1) == '\0') return 0; // Invalid format

    for (char *c = input; c < equal_pos; c++) {
        if (*c == ' ') return 0; // Invalid space before '='
    }

    return 1;
}

// Function to handle variable assignment
void handle_variable_assignment(char *input) {
    char *equal_pos = strchr(input, '=');
    *equal_pos = '\0';

    char *name = input;
    char *value = equal_pos + 1;

    set_variable(name, value);
}

// Function to free memory
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
        printf("Nano Shell >>> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = 0; // Remove newline

        if (strlen(input) == 0) continue;

        if (is_variable_assignment(input)) {
            handle_variable_assignment(input);
            continue;
        }

        int arg_count;
        char **args = parse_input(input, &arg_count);
        substitute_variables(args, arg_count);

        if (args[0] != NULL) {
            if (!execute_builtin(args, arg_count)) {
                execute_external(args);
            }
        }

        free_args(args);
    }

    return 0;
}
