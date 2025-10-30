#define NOB_IMPLEMENTATION
#define NOB_WARN_DEPRECATED

#include "nob.h"

#define BUILD_FOLDER "build/"
#define SOURCE_FOLDER "src/"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) {
        return 1;
    }

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-Wpedantic", "-c",
                   SOURCE_FOLDER "todo.c", "-o", BUILD_FOLDER "todo.o");

    if (!nob_cmd_run(&cmd))
        return 1;

    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-Wpedantic", "-c",
                   SOURCE_FOLDER "main.c", "-o", BUILD_FOLDER "main.o");

    if (!nob_cmd_run(&cmd))
        return 1;

    nob_cmd_append(&cmd, "cc", BUILD_FOLDER "main.o", BUILD_FOLDER "todo.o",
                   "-o", "toci");

    if (!nob_cmd_run(&cmd))
        return 1;

    return 0;
}
