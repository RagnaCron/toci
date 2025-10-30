#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "todo.h"

static int hasNoSubcommand(const char *command);
static void printUsage(void);
static void printInfo(void);

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

    if (strcmp(argv[1], "new") == 0 && argc == 3) {
        return newTodos(argv[2]);
    }

    if (strcmp(argv[1], "fix") == 0 && argc == 3) {
        return fixTodos(argv[2]);
    }

    if (strcmp(argv[1], "list") == 0 && argc == 4) {
        if (hasNoSubcommand(argv[3])) {
            return EXIT_FAILURE;
        }

        return listTodos(argv[2], argv[3]);
    }

    if (strcmp(argv[1], "check") == 0 && argc == 4) {
        if (hasNoSubcommand(argv[3])) {
            return EXIT_FAILURE;
        }

        return checkTodos(argv[2], argv[3], true);
    }

    if (strcmp(argv[1], "uncheck") == 0 && argc == 4) {
        if (hasNoSubcommand(argv[3])) {
            return EXIT_FAILURE;
        }

        return checkTodos(argv[2], argv[3], false);
    }

    if (strcmp(argv[1], "delete") == 0 && argc == 4) {
        if (hasNoSubcommand(argv[3])) {
            return EXIT_FAILURE;
        }

        return deleteTodos(argv[2], argv[3]);
    }

    fprintf(stderr, "Uups, you messed up, try again!\n");
    printUsage();
    return EXIT_FAILURE;
}

static int hasNoSubcommand(const char *command) {
    if (command == NULL) {
        fprintf(stderr, "Missing subcommand.\n");
        printUsage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS; // return 0
}

static void printUsage(void) {
    printf("Usage: toci <command> [option]\n"
           "\tCommands:\n"
           "\thelp\t\tThis help text.\n"
           "\tinfo\t\tAdditional information on usage.\n"
           "\tnew <filename>\tThis will create a new todo file or append.\n"
           "\t\t\tto an existing one.\n"
           "\tfix <filename>\tThis will prefix a checkbox if one is missing.\n"
           "\tlist <filename> [checked|unchecked|all]\n"
           "\t\t\tThis will list todos depending on the subcommand.\n"
           "\tcheck <filename> [linenumber|all]\n"
           "\t\t\tThis will check a todo item depening on the subcommand.\n"
           "\tuncheck <filename> [linenumber|all]\n"
           "\t\t\tThis will uncheck a todo item depending on the subcommand.\n"
           "\tdelete <filename> [linenumber|all]\n"
           "\t\t\tThis will delete a todo item depending on the subcommand.\n"
           "\n");
}

static void printInfo(void) {
    printf("Info:\n"
           "\tThe todo should be on a single line.\n"
           "\tThe line length should not be more then 121 characters or\n"
           "\tit will be cut of...\n\n");
}
