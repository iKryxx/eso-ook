//
// Created by ikryxxdev on 1/14/26.
//

#define forall(i, n) for(size_t i = 0; i < n; ++i)

#include "ook.h"

#include <assert.h>
#include <stdio.h>

#include "dynarr.h"

#include <string.h>

int ook_debug = 0;

int ook_inline = 0;

char* read_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(size + 1);
    if (!buffer) {
        perror("Failed to allocate memory for file");
        exit(1);
    }
    fread(buffer, size, 1, file);
    buffer[size] = '\0';
    fclose(file);
    return buffer;
}

char match_token(const char *code) {
    if (strncmp(code, "Ook. Ook?", 9) == 0) return TK_PTR_R;
    if (strncmp(code, "Ook? Ook.", 9) == 0) return TK_PTR_L;
    if (strncmp(code, "Ook. Ook.", 9) == 0) return TK_INC;
    if (strncmp(code, "Ook! Ook!", 9) == 0) return TK_DEC;
    if (strncmp(code, "Ook! Ook.", 9) == 0) return TK_OUTPUT;
    if (strncmp(code, "Ook. Ook!", 9) == 0) return TK_INPUT;
    if (strncmp(code, "Ook! Ook?", 9) == 0) return TK_LOOP_START;
    if (strncmp(code, "Ook? Ook!", 9) == 0) return TK_LOOP_END;
    if (*code == '\0') return TK_EOF;
    return (char)-1;
}

ook_program_t parse_program(const char *code) {
    ook_program_t program = {
        .code = {NULL},
        .memory = {NULL}
    };
    char prev = 0;
    size_t count = 1;

    size_t loop_stack[1024];
    size_t sp = 0;

    forall(i, strlen(code) + 1) {
        char c = match_token(code + i);
        switch (c) {
            case TK_LOOP_START: {
                if (prev != '\0') {
                    dynarr_append(&program.code, ((ook_token_t){prev, count}));
                    count = 1;
                }
                // Placeholder
                size_t idx_open = program.code.count;
                dynarr_append(&program.code, ((ook_token_t){TK_LOOP_START, 0}));
                if (sp >= sizeof(loop_stack) / sizeof(loop_stack[0])) {
                    printf("Loop nesting too deep at %zu\n", i);
                    exit(1);
                }
                loop_stack[sp++] = idx_open;
                break;
            }
            case TK_LOOP_END: {
                if (prev != '\0') {
                    dynarr_append(&program.code, ((ook_token_t){prev, count}));
                    count = 1;
                }

                if (sp == 0) {
                    printf("Unmatched loop end at %zu\n", i);
                    exit(1);
                }

                size_t idx_open = loop_stack[--sp];
                size_t idx_close = program.code.count;

                dynarr_append(&program.code, ((ook_token_t){TK_LOOP_END, idx_open + 1}));
                program.code.items[idx_open].data = idx_close + 1;
                break;
            }

            case TK_INC:
            case TK_DEC:
            case TK_PTR_L:
            case TK_PTR_R:
            case TK_INPUT:
            case TK_OUTPUT:
            case TK_EOF:
                if (c == prev) {
                    count++;
                }
                else if (prev != TK_EOF && prev != TK_LOOP_START && prev != TK_LOOP_END) {
                    dynarr_append(&program.code, ((ook_token_t){prev, count}));
                    count = 1;
                }
                break;
            default:
                continue;
        }
        i += 8;
        prev = c;
    }
    return program;
}

void run_program(ook_program_t *program) {
    dynarr_reserve(&program->memory, 32*32);
    uint16_t sp = 0;
    forall(i, program->code.count) {
        ook_token_t token = program->code.items[i];
        switch (token.type) {
            case TK_INC: program->memory.items[sp] += token.data; break;
            case TK_DEC: program->memory.items[sp] -= token.data; break;
            case TK_PTR_L: sp -= token.data; break;
            case TK_PTR_R: sp += token.data; break;
            case TK_INPUT: program->memory.items[sp] = getchar(); break;
            case TK_OUTPUT: if (!ook_debug) for (uint64_t j = 0; j < token.data; j++) putchar(program->memory.items[sp]); break;
            case TK_LOOP_START: if (program->memory.items[sp] == 0) i = token.data - 1; break;
            case TK_LOOP_END: i = token.data - 2; break;
            default: assert(false);
        }
        if (ook_debug) {
            system("clear");
            print_memory(&program->memory);
        }
    }
}

void print_memory(const ook_memory_t *memory) {

    for(uint32_t i = 0; i < memory->capacity; i++) {
        if ( i % 32 == 0) printf("\n%4x ", i);
        printf("[%4hhu] ", memory->items[i]);
    }
    printf("\n");
    fflush(stdout);
}
