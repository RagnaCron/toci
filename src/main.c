#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 128
#define LINE 121

#define CHECKBOX_UNCHECKED "[] "
#define CHECKBOX_CHECKED "[X] "

void printUsage(void);
int listTodos(const char *file_name);
int newTodos(const char *file_name);
int fixTodos(const char *file_name);

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

    if (strcmp(argv[1], "fix") == 0 && argc == 3) {
        return fixTodos(argv[2]);
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
           "\tfix <filename>\tThis will add a checkbox if one is missing.\n"
           "\nInfo:\n"
           "\tThe todo should be on a single line.\n"
           "\tThe line length should not be more then 121 characters\n"
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

    if (c == EOF) {
        flush_stdin(); // to leave a clean stdin state
    }

    return c == EOF ? EOF : i;
}

static bool hasCheckbox(char line[]) {
    return (strncmp(line, CHECKBOX_UNCHECKED, strlen(CHECKBOX_UNCHECKED)) ==
                0 ||
            strncmp(line, CHECKBOX_CHECKED, strlen(CHECKBOX_CHECKED)) == 0);
}

static void ensureCheckboxPrefix(char line[]) {
    if (hasCheckbox(line)) {
        return;
    }

    char new_line[MAX_LINE];
    strcpy(new_line, CHECKBOX_UNCHECKED);
    strcat(new_line, line);
    strcpy(line, new_line);
}

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
            printf("The line is to long. Only use 121 characters.\n");
            continue;
        }

        ensureCheckboxPrefix(line);

        fprintf(fptr, "%s", line);
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}

static void trimNewLine(char line[]) {
    if (strchr(line, '\n') != NULL) {
        line[strcspn(line, "\n")] = '\0';
    }
}

static int handleLongLine(char line[]) {
    int state = 0;

    for (;;) {
        printf("\nLine to long (%zu chars):\n", strlen(line));
        printf("%s", line);
        printf("\n\n");
        printf("Enter <ctrl + D> to remove this line.\n");
        printf("Enter a shorter version (max %d chars):\n", LINE);

        fflush(stdout);

        char new_line[MAX_LINE];
        state = read_line(new_line);
        strcpy(new_line, line);

        if (state == EOF) {
            clearerr(stdin);
            break;
        }

        if (state == LINE) {
            break;
        }
    }

    return state;
}

static void discardLine(FILE *in) {
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, in))
        ;
}

int fixTodos(const char *file_name) {
    FILE *in = fopen(file_name, "r");
    FILE *out = fopen("todo.tmp", "w");
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    char line[MAX_LINE + 1];
    while (fgets(line, MAX_LINE + 1, in)) {
        if (strlen(line) > MAX_LINE) {
            discardLine(in);
        }
        trimNewLine(line);

        if (strlen(line) > LINE) {
            int state = handleLongLine(line);
            if (state == EOF) {
                continue;
            }
        }

        if (!hasCheckbox(line)) {
            ensureCheckboxPrefix(line);
        }

        fprintf(out, "%s\n", line);
    }

    fclose(in);
    fclose(out);

    remove(file_name);
    rename("todo.tmp", file_name);

    return EXIT_SUCCESS;
}
