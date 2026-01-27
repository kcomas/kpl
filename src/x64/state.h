
#pragma once

#include "./queue.h"

typedef struct {
    int32_t byte_start, byte_pos, data_pos, data_size;
    ssize_t next_label; // -1 not set
    type_table *next_fn;
    map *queue;
} x64_state;

void x64_state_init(x64_state *state, int32_t byte_start, map *queue);

void x64_state_free(x64_state *state);

void x64_state_unlock_mem(x64_state *state);

void x64_state_lock_mem(x64_state *state);

#define X64_STATE_PRINT(NAME) X64_STATE_PRINT_##NAME

typedef enum [[gnu::packed]] {
    X64_STATE_PRINT(NL_END)     = 1 << 0,
    X64_STATE_PRINT(BYTES)      = 1 << 1,
    X64_STATE_PRINT(_)          = 0
} x64_state_print_opts;

void x64_state_print(const x64_state *state, FILE *file, int32_t idnt, x64_state_print_opts print_opts);

#define OP_NEXT_SIZE 2

typedef struct {
    x64_queue_size label_size;
    int8_t mod, r, rm, scale, index, base; // -1 not used
    uint8_t rex, dsp_byte_size, rel_byte_size, imm_byte_size, op_len; // 0 not used
    int8_t reg[X64_OP_SIZE]; // -1 not used
    int16_t po, next[OP_NEXT_SIZE]; // -1 not used
    x64_pfx_flag pfx;
    int32_t dsp, rel;
    int32_t byte_start, byte_end; // -1 not used
    x64_op_reg op[X64_OP_SIZE];
    ssize_t label; // -1 not used
    def_data imm;
    const x64_inst *inst;
} x64_op;

inline x64_op x64_op_init(void) {
    return (x64_op) {
        .label_size = X64_QUEUE_SIZE(_),
        .mod = -1, .r = -1, .rm = -1, .scale = -1, .index = -1, .base = -1,
        .rex = 0, .dsp_byte_size = 0, .rel_byte_size = 0, .imm_byte_size = 0, .op_len = 0,
        .reg = { -1, -1, -1, -1 },
        .po = -1, .next = { -1, -1 },
        .pfx = X64_PFX_FLAG(_),
        .dsp = 0, .rel = 0,
        .byte_start = -1, .byte_end = -1,
        .op = { X64_OP_REG(_), X64_OP_REG(_), X64_OP_REG(_), X64_OP_REG(_) },
        .label = -1,
        .imm = DEF(_),
        .inst = nullptr,
    };
}

inline void x64_op_reset(x64_op *op) {
    op->label_size = X64_QUEUE_SIZE(_);
    op->mod = op->r = op->rm = op->scale = op->index = op->base = -1;
    op->rex = op->dsp_byte_size = op->rel_byte_size = op->imm_byte_size = op->op_len = 0;
    op->po = op->next[0] = op->next[1] = -1;
    op->pfx = X64_PFX_FLAG(_);
    op->dsp = op->rel = 0;
    op->byte_start = op->byte_end = -1;
    for (uint8_t op_idx = 0; op_idx < X64_OP_SIZE; op_idx++) {
        op->reg[op_idx] = -1;
        op->op[op_idx] = X64_OP_REG(_);
    }
    op->label = -1;
    op->imm = DEF(_);
    op->inst = nullptr;
}

#define X64_OP_PRINT(NAME) X64_OP_PRINT_##NAME

typedef enum [[gnu::packed]] {
    X64_OP_PRINT(NL_END)        = 1 << 0,
    X64_OP_PRINT(ASSEMBLE)      = 1 << 1,
    X64_OP_PRINT(DEBUG)         = 1 << 2,
    X64_OP_PRINT(_)             = 0
} x64_op_print_opts;

void x64_op_print(const x64_op *op, FILE *file, int32_t idnt, x64_op_print_opts print_opts);
