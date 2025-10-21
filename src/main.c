#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 254

void printUsage(void);
int listTodos(const char *file_name);
int newTodos(const char *file_name);

int main(const int argc, const char *argv[]) {
    if (argc < 2) {
        printUsage();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "help") == 0) {
        printUsage();
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "list") == 0 && argc == 3) {
        return listTodos(argv[2]);
    }

    if (strcmp(argv[1], "new") == 0 && argc == 3) {
        return newTodos(argv[2]);
    }

    return EXIT_SUCCESS;
}

void printUsage(void) {
    printf("Usage: toci <command>\n");
    printf("\tCommands:\n");
    printf("\thelp\t\tThis is it...\n");
    printf("\tlist <filename>\tThis will list all todos.\n");
    printf("\tnew <filename>\tThis will create a new todo file or\n\t\t\tappend to an existing one.\n");
    printf("\n");
}

int listTodos(const char *file_name) {
    FILE *fptr = fopen(file_name, "r");

    if (fptr == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    char line[MAX_LINE + 1];
    while (fgets(line, MAX_LINE, fptr)) {
        printf("%s", line);
    }

    fclose(fptr);

    return EXIT_SUCCESS;
}

int newTodos(const char *file_name) {
    FILE *fptr = fopen(file_name, "a");
    if (fptr == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    char line[MAX_LINE + 1];
    while (fgets(line, MAX_LINE, stdin)) {
        if (line[0] == '[') {
            fprintf(fptr, "%s", line);
        } else {
            fprintf(fptr, "[] %s", line);
        }
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}
