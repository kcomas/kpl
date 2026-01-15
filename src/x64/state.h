
#pragma once

#include "./queue.h"

typedef struct {
    int32_t byte_start, byte_pos;
    ssize_t next_label; // -1 not set
    type_table *next_fn;
    map *queue;
} x64_state;

void x64_state_init(x64_state *state, int32_t byte_start, map *queue);

void x64_state_free(x64_state *state);

#define X64_STATE_PRINT(NAME) X64_STATE_PRINT_##NAME

typedef enum [[gnu::packed]] {
    X64_STATE_PRINT(NL_END)     = 1 << 0,
    X64_STATE_PRINT(_)          = 0
} x64_state_print_opts;

void x64_state_print(const x64_state *state, FILE *file, int32_t idnt, x64_state_print_opts print_opts);

typedef struct {
    int8_t rex, r, rm, scale, index; // -1 not used
    uint8_t dsp_byte_size, imm_byte_size; // 0 not used
    x64_pfx_flag pfx_flag;
    int32_t dsp;
    int32_t start_byte, end_byte; // -1 not used
    x64_op_reg op[X64_OP_SIZE];
    ssize_t label; // -1 not used
    def_data imm;
    const x64_inst *inst;
    x64_state *state;
} x64_op;

inline x64_op x64_op_init(void) {
    return (x64_op) {
        .rex = -1, .r = -1, .rm = -1, .scale = -1, .index = -1,
        .dsp_byte_size = 0, .imm_byte_size = 0,
        .pfx_flag = X64_PFX_FLAG(_),
        .dsp = 0,
        .start_byte = -1, .end_byte = -1,
        .op = { X64_OP_REG(_), X64_OP_REG(_), X64_OP_REG(_), X64_OP_REG(_) },
        .label = -1,
        .imm = DEF(_),
        .inst = nullptr,
        .state = nullptr
    };
}

inline void x64_op_reset(x64_op *op) {
    op->rex = op->r = op->rm = op->scale = op->index = -1;
    op->dsp_byte_size = op->imm_byte_size = 0;
    op->pfx_flag = X64_PFX_FLAG(_);
    op->dsp = 0;
    op->start_byte = op->end_byte = -1;
    for (uint8_t op_idx = 0; op_idx < X64_OP_SIZE; op_idx++)
        op->op[op_idx] = X64_OP_REG(_);
    op->label = -1;
    op->imm = DEF(_);
    op->inst = nullptr;
}

#define X64_OP_PRINT(NAME) X64_OP_PRINT_##NAME

typedef enum [[gnu::packed]] {
    X64_OP_PRINT(NL_END)    = 1 << 0,
    X64_OP_PRINT(_)         = 0
} x64_op_print_opts;

void x64_op_print(const x64_op *op, FILE *file, int32_t idnt, x64_op_print_opts print_opts);
