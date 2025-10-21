#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHelp(void);
int  listTodos(const char* file_name);

int main(const int argc, const char* argv[]) {
    if (argc < 2) {
        printHelp();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[argc - 2], "list") == 0) {
        return listTodos(argv[argc - 1]);
    }

    return EXIT_SUCCESS;
}

void printHelp(void) {}

int listTodos(const char* file_name) {
    FILE* fptr = fopen(file_name, "r");

    if (fptr == NULL) {
        perror("Error opening file");
        fprintf(stderr, "Error code: %d\n", errno);
        return errno;
    }

    char line[1000];
    while (fgets(line, 999, fptr)) {
        printf("%s", line);
    }

    fclose(fptr);

    return EXIT_SUCCESS;
}
