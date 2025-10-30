#define NOB_IMPLEMENTATION
#define NOB_WARN_DEPRECATED
#define NOB_EXPERIMENTAL_DELETE_OLD

#include "nob.h"

#define BUILD_FOLDER "build/"
#define SOURCE_FOLDER "src/"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) {
        return 1;
    }

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-Wpedantic", "-o", "toci",
                   SOURCE_FOLDER "main.c", SOURCE_FOLDER "todo.c");

    if (!nob_cmd_run(&cmd))
        return 1;

    return 0;
}
