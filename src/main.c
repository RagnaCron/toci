#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 255
#define LINE 127

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
    printf("Usage: toci <command>\n"
           "\tCommands:\n"
           "\thelp\t\tThis is it...\n"
           "\tlist <filename>\tThis will list all todos.\n"
           "\tnew <filename>\tThis will create a new todo file or\n"
           "\t\t\tappend to an existing one.\n"
           "\nInfo:\n"
           "\tThe todo should be on a single line.\n"
           "\tThe line length should not be more then 254 characters\n"
           "\tor it will be cut of...\n"
           "\n");
}

int listTodos(const char *file_name) {
    FILE *fptr = fopen(file_name, "r");

    if (fptr == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, fptr)) {
        printf("%s", line);
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}

static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int read_line(char line[]) {
    char c;
    int i = 0;
    while ((c = getchar()) != EOF) {
        if (i < LINE) {
            line[i++] = c;
            if (c == '\n') {
                break;
            }
        } else {
            // discard rest of input
            flush_stdin();
            break;
        }
    }
    line[i] = '\0';

    return c == EOF ? EOF : i;
}

void check_todo(char line[]) {}

int newTodos(const char *file_name) {
    FILE *fptr = fopen(file_name, "a");
    if (fptr == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    char line[MAX_LINE];
    for (;;) {
        printf("New Todo > ");

        int i = read_line(line);

        if (i == EOF) {
            printf("\n");
            break;
        }

        if (i == LINE) {
            printf("The line is to long. Only use 128 characters.\n");
            continue;
        }

        check_todo(line);

        // fprintf(fptr, "%s", line);
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}
