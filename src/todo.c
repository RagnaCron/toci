#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "todo.h"


static bool is_checked(const char line[]) {
    return strncmp(line, CHECKBOX_CHECKED, strlen(CHECKBOX_CHECKED)) == 0;
}

int list_todos(const char *file_name, const char *option) {
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
        } else if (strcmp(option, "checked") == 0 && is_checked(line)) {
            printf("%3d %s", line_number, line);
        } else if (strcmp(option, "unchecked") == 0 && !is_checked(line)) {
            printf("%3d %s", line_number, line);
        }
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}

static bool is_number(const char *option, long *number) {
    char *endptr;

    *number = strtol(option, &endptr, 10);

    if (*endptr != '\0') {
        perror("Invalid integer");
        fprintf(stderr, "Invalid integer: '%s'\n", option);
        return false;
    }

    return true;
}

static int check_todo(char *line, char *checkbox_str) {
    char new_line[MAX_LINE];
    strcpy(new_line, checkbox_str);

    int index = 0;
    int pos = 1;

    while (line[pos + index] != '\n') {
        new_line[1 + index] = line[pos + index];
        index++;
    }
    new_line[1 + index++] = '\n';
    new_line[1 + index] = '\0';

    strcpy(line, new_line);

    return EXIT_SUCCESS;
}

int check_todos(const char *file_name, const char *option, bool isCheck) {
    long number = 0;
    bool isAll = strcmp(option, "all") == 0;
    if (!isAll) {
        if (!is_number(option, &number)) {
            fprintf(stderr, "Passed the wrong subcommand...\n");
            return EXIT_FAILURE;
        }
    }

    FILE *in = fopen(file_name, "r");
    FILE *out = fopen("todo.tmp", "w");

    if (in == NULL || out == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    int state = EXIT_SUCCESS;
    int line_number = 0;
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, in)) {
        if (state != EXIT_SUCCESS) {
            break;
        }

        ++line_number;

        if (number > 0 && number == line_number) {
            if (isCheck && !is_checked(line)) {
                state = check_todo(line, CHECKBOX_CHECKED);
            } else if (!isCheck && is_checked(line)) {
                state = check_todo(line, CHECKBOX_UNCHECKED);
            }
        } else if (isAll) {
            if (isCheck && !is_checked(line)) {
                state = check_todo(line, CHECKBOX_CHECKED);
            } else if (!isCheck && is_checked(line)) {
                state = check_todo(line, CHECKBOX_UNCHECKED);
            }
        }

        fprintf(out, "%s", line);
    }

    fclose(in);
    fclose(out);

    if (state == EXIT_SUCCESS) {
        remove(file_name);
        rename("todo.tmp", file_name);
    } else {
        remove("todo.tmp");
    }

    return state;
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

static bool has_checkbox(char line[]) {
    return (strncmp(line, CHECKBOX_UNCHECKED, strlen(CHECKBOX_UNCHECKED)) ==
                0 ||
            strncmp(line, CHECKBOX_CHECKED, strlen(CHECKBOX_CHECKED)) == 0);
}

static void ensure_checkbox_prefix(char line[]) {
    if (has_checkbox(line)) {
        return;
    }

    char new_line[MAX_LINE];
    strcpy(new_line, CHECKBOX_UNCHECKED);
    strcat(new_line, line);
    strcpy(line, new_line);
}

int new_todos(const char *file_name) {
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

        ensure_checkbox_prefix(line);

        fprintf(fptr, "%s", line);
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}

static void trim_new_line(char line[]) {
    if (strchr(line, '\n') != NULL) {
        line[strcspn(line, "\n")] = '\0';
    }
}

static void discard_line(FILE *in) {
    int c;
    while ((c = fgetc(in)) != '\n' && c != EOF)
        printf("%c", (char)c);
}

static int handle_long_line(char line[]) {
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
            trim_new_line(new_line);
            strcpy(line, new_line);
            break;
        }
    }

    return state;
}

static bool remove_todo(const char *line) {
    printf("%s", line);
    printf("Remove the line, y/n (default): ");
    char c = getchar();
    if (c == 'y')
        return true;
    else
        return false;
}

int delete_todos(const char *file_name, const char *option) {
    long number = 0;
    bool isAll = strcmp(option, "all") == 0;
    bool isInteractive = strcmp(option, "i") == 0;
    if (!isAll) {
        if (!isInteractive) {
            if (!is_number(option, &number)) {
                fprintf(stderr, "Passed the wrong subcommand...\n");
                return EXIT_FAILURE;
            }
        }
    }

    FILE *in = fopen(file_name, "r");
    FILE *out = fopen("todo.tmp", "w");
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    int state = EXIT_SUCCESS;

    if (!isAll) {
        int line_number = 0;
        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, in)) {
            ++line_number;

            if (number > 0 && number == line_number) {
                continue;
            } else if (isInteractive) {
                if (remove_todo(line)) {
                    continue;
                }
            }

            fprintf(out, "%s", line);
        }
    }

    fclose(in);
    fclose(out);

    if (state == EXIT_SUCCESS) {
        remove(file_name);
        rename("todo.tmp", file_name);
    } else {
        remove("todo.tmp");
    }

    return state;
}

int fix_todos(const char *file_name) {
    FILE *in = fopen(file_name, "r");
    FILE *out = fopen("todo.tmp", "w");
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, in)) {
        if (strlen(line) == (MAX_LINE)) {
            discard_line(in);
        }
        trim_new_line(line);

        if (strlen(line) > LINE) {
            const int state = handle_long_line(line);
            if (state == EOF) {
                continue;
            }
        }

        if (!has_checkbox(line)) {
            ensure_checkbox_prefix(line);
        }

        fprintf(out, "%s\n", line);
    }

    fclose(in);
    fclose(out);

    remove(file_name);
    rename("todo.tmp", file_name);

    return EXIT_SUCCESS;
}
