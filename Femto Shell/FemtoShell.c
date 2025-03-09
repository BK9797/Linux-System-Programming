#include <stdio.h>
#include <string.h>

#define MAX_INPUT 256

int main() {
    char input[MAX_INPUT];

    while (1) {
        printf("Femto Shell >>> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
        } else if (strcmp(input, "exit") == 0) {
            printf("Good Bye :)\n");
            break;
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
