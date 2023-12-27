#define NOB_IMPLEMENTATION
#include "nob.h"

void log_available_subcommands(const char *program, Nob_Log_Level level)
{
    nob_log(level, "Usage: %s <subcommand>", program);
    nob_log(level, "Subcommands:");
    nob_log(level, "    run");
    nob_log(level, "    build");
    nob_log(level, "    clean");
}

void build(void) {
    Nob_Cmd cmd = {0};
    nob_log(NOB_INFO, "building...");
    nob_mkdir_if_not_exists("build");
    nob_cmd_append(&cmd,"gcc", "-Wall", "-Wextra", "main.c", "-o", "build/main");
    nob_cmd_run_sync(cmd);
}

int main(int argc, char** argv) {

    NOB_GO_REBUILD_URSELF(argc, argv);

    const char* program = nob_shift_args(&argc, &argv);

    if(argc <= 0) {
        log_available_subcommands(program, NOB_INFO);
        nob_log(NOB_ERROR, "no subcommand provided");
        return 1;
    }

    const char* subcommand = nob_shift_args(&argc, &argv);

    Nob_Cmd cmd = {0};

    if(strcmp(subcommand, "build") == 0) {
        build();
    } else if(strcmp(subcommand, "run") == 0) {
        build();
        nob_cmd_append(&cmd,"./build/main");
        nob_da_append_many(&cmd, argv, argc);
        nob_cmd_run_sync(cmd);
    } else if (strcmp(subcommand, "clean") == 0) {
        nob_log(NOB_INFO, "cleaning...");
        nob_cmd_append(&cmd,"rm", "-r", "build");
        nob_cmd_run_sync(cmd);
    } else {
        nob_log(NOB_ERROR, "Unknown subcommand '%s'", subcommand);
        log_available_subcommands(program, NOB_INFO);
        return 1;
    }

    return 0;
}