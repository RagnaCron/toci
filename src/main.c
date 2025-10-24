#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "todo.h"

void printUsage(void);
void printInfo(void);

int main(const int argc, const char *argv[]) {
    if (argc < 2) {
        printUsage();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "help") == 0) {
        printUsage();
        return EXIT_SUCCESS;
    }

    // todo: this will need some more information...for its info...
    if (strcmp(argv[1], "info") == 0) {
        printInfo();
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "list") == 0 && argc >= 3) {
        return listTodos(argv[2], argv[3]);
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
    printf("Usage: toci <command> [option]\n"
           "\tCommands:\n"
           "\thelp\t\tThis help text.\n"
           "\tinfo\t\tAdditional information on usage.\n"
           "\tlist <filename> [checked|unchecked]\n"
           "\t\t\tThis will list todos, not using an option will show all.\n"
           "\tnew <filename>\tThis will create a new todo file or append\n"
           "\t\t\tto an existing one.\n"
           "\tfix <filename>\tThis will add a checkbox if one is missing.\n"
           "\n");
}

void printInfo(void) {
    printf("Info:\n"
           "\tThe todo should be on a single line.\n"
           "\tThe line length should not be more then 121 characters or\n"
           "\tit will be cut of...\n\n");
}
