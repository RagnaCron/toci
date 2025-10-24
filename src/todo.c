#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "todo.h"


static bool isChecked(const char line[]) {
    return strncmp(line, CHECKBOX_CHECKED, strlen(CHECKBOX_CHECKED)) == 0;
}

int listTodos(const char *file_name, const char *option) {
    FILE *fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    int line_number = 0;
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, fptr)) {
        ++line_number;
        if (option == NULL) {
            printf("%3d %s", line_number, line);
        } else if (strcmp(option, "checked") == 0 && isChecked(line)) {
            printf("%3d %s", line_number, line);
        } else if (strcmp(option, "unchecked") == 0 && !isChecked(line)) {
            printf("%3d %s", line_number, line);
        }
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}

int checkTodos(const char *file_name, const char *option) {

    FILE *in = fopen(file_name, "r");
    FILE *out = fopen("todo.tmp", "w");

    if (in == NULL || out == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    // if line number, check todo given (option) line number
    // else if all, check all todos

    int line_number = 0;
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, in)) {}

    fclose(in);
    fclose(out);

    remove(file_name);
    rename("todo.tmp", file_name);

    return EXIT_SUCCESS;
}

static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int read_line(char line[]) {
    int c;
    int i = 0;
    while ((c = getchar()) != EOF) {
        if (i < LINE) {
            line[i++] = (char)c;
            if (c == '\n') {
                break;
            }
        } else {
            // discard the rest of input
            flush_stdin();
            break;
        }
    }
    line[i] = '\0';

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

    for (;;) {
        char line[MAX_LINE];
        printf("New Todo > ");

        const int state = read_line(line);

        if (state == EOF) {
            printf("\n");
            break;
        }

        if (state == LINE) {
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

static void discardLine(FILE *in) {
    int c;
    while (fgetc(in) != '\n' && c != EOF)
        printf("%c", (char)c);
}

static int handleLongLine(char line[]) {
    int state = 0;

    for (;;) {
        printf("\nLine to long (%zu chars):\n", strlen(line));
        printf("%s", line);
        printf("\n\n");
        printf("Enter <ctrl + D> to remove this line.\n");
        printf("Enter a shorter version (max %d chars):\n", LINE);

        char new_line[MAX_LINE];
        state = read_line(new_line);

        if (state == EOF) {
            clearerr(stdin);
            break;
        }

        if (state <= LINE) {
            trimNewLine(new_line);
            strcpy(line, new_line);
            break;
        }
    }

    return state;
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
        if (strlen(line) == (MAX_LINE)) {
            discardLine(in);
        }
        trimNewLine(line);

        if (strlen(line) > LINE) {
            const int state = handleLongLine(line);
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
