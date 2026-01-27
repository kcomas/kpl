
#include "./core/core.h"

static int16_t inst_idx_array[X64_MAX_OPCODE_COUNT] = {};

static void inst_idx_array_reset(void) {
    for (int16_t array_idx = 0; array_idx < X64_MAX_OPCODE_COUNT; array_idx++)
        inst_idx_array[array_idx] = -1;
}

static void inst_idx_array_print(void) {
    int16_t array_idx = 0;
    while (inst_idx_array[array_idx] != -1)
        printf("%d, ", inst_idx_array[array_idx++]);
    printf("%d },\n", -1);
}

int main(void) {
    printf("\n// GENERATED CODE DO NOT EDIT\n\n");
    printf("#include \"./table.h\"\n\n");
    printf("const int16_t *x64_mne_table[] = {\n");
    for (int16_t mne_idx = X64_MNE_INST(ADD); mne_idx < X64_MNE_INST(_); mne_idx++) {
        inst_idx_array_reset();
        int16_t inst_idx = 0, array_idx = 0;
        while (!(x64_inst_table[inst_idx].flags & X64_FLAG(END))) {
            if (x64_inst_table[inst_idx].mne == mne_idx)
                inst_idx_array[array_idx++] = inst_idx;
            inst_idx++;
        }
        printf("    [X64_MNE_INST(%s)] = (int16_t[]) { ", x64_mne_str(mne_idx));
        inst_idx_array_print();
    }
    printf("};\n\n");
    printf("const int16_t *x64_opcode_table[] = {\n");
    for (int16_t opcode_idx = 0; opcode_idx <= 0x1FF; opcode_idx++) {
        inst_idx_array_reset();
        int16_t inst_idx = 0, array_idx = 0;
        while (!(x64_inst_table[inst_idx].flags & X64_FLAG(END))) {
            if (x64_inst_table[inst_idx].mne == X64_MNE(INVALID) &&
                    !(x64_inst_table[inst_idx].flags & X64_FLAG(DISASSEMBLER))) {
                inst_idx++;
                continue;
            }
            if (opcode_idx <= 0xFF && !(x64_inst_table[inst_idx].flags & X64_FLAG(0F))) {
                if (x64_inst_table[inst_idx].po == opcode_idx)
                    inst_idx_array[array_idx++] = inst_idx;
            } else if (opcode_idx > 0xFF && (x64_inst_table[inst_idx].flags & X64_FLAG(0F))) {
                if (x64_inst_table[inst_idx].po == opcode_idx % 0x100)
                    inst_idx_array[array_idx++] = inst_idx;
            }
            inst_idx++;
        }
        printf("    [0x%02X] = (int16_t[]) { ", opcode_idx);
        inst_idx_array_print();
    }
    printf("};\n");
    return 0;
}
