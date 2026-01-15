#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ook.h"
#include "dynarr.h"

const char* USAGE =
    "Usage: ook [<file>] [<options>]\n"
    "Options:\n"
    "-h --help:\t\tPrints this help.\n"
    "-d --debug:\t\tPrints the memory. This won't print stdout.\n"
    "-i --inline [code]\n"
;

const char* FILE_PATH = NULL;

const char* INLINED_CODE = NULL;

void parse_args(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        const char *arg = argv[i];
        if (i == 1 && arg[0] != '-') {
            FILE_PATH = arg;
        }
        else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            printf("%s", USAGE);
            exit(0);
        }
        else if (strcmp(arg, "-d") == 0 || strcmp(arg, "--debug") == 0) {
            ook_debug = 1;
        }
        else if (strcmp(arg, "-i") == 0 || strcmp(arg, "--inline") == 0) {
            ook_inline = 1;
            if (argc <= i + 1) {
                printf("No code specified for inline\n");
                exit(1);
            }
            INLINED_CODE = argv[++i];
        }
        else {
            printf("Unknown argument: %s\n", arg);
            exit(1);
        }
    }
}

int main(int argc, char **argv) {

    ook_program_t program;
    parse_args(argc, argv);
    if (INLINED_CODE) {
        program = parse_program(INLINED_CODE);
    }
    else if (FILE_PATH) {
        char* code = read_file(FILE_PATH);
        program = parse_program(code);
        free(code);
    }
    else {
        printf("%s\n", USAGE);
        printf("No file specified\n");
        exit(1);
    }

    run_program(&program);
    dynarr_free(&program.code);
    dynarr_free(&program.memory);
    return 0;
}