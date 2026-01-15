//
// Created by ikryxxdev on 1/14/26.
//

#ifndef OOK_OOK_H
#define OOK_OOK_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    TK_INC = '+',
    TK_DEC = '-',
    TK_PTR_L = '<',
    TK_PTR_R = '>',
    TK_LOOP_START = '[',
    TK_LOOP_END = ']',
    TK_INPUT = ',',
    TK_OUTPUT = '.',
    TK_EOF = '\0'
} ook_token_type_t;

typedef struct {
    ook_token_type_t type;
    uint64_t data;
} ook_token_t;

typedef struct {
    ook_token_t *items;
    size_t count;
    size_t capacity;
} ook_token_array_t;

typedef struct {
    uint8_t *items;
    size_t count;
    size_t capacity;
} ook_memory_t;

typedef struct {
    ook_token_array_t code;
    ook_memory_t memory;
} ook_program_t;

extern int ook_debug;

extern int ook_inline;

extern char* read_file(const char* path);

extern ook_program_t parse_program(const char* code);

extern void run_program(ook_program_t* program);

extern void print_memory(const ook_memory_t* memory);


#endif //OOK_OOK_H