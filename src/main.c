
#include "./core/core.h"

static const char version[] = {
#embed "./VERSION"
, '\0' };

int usage(const char *argv0) {
    printf(COLOR(BOLD) "KPL %s" COLOR(RESET), version);
    printf(COLOR(BOLD) "Usage: %s file.kpl\n" COLOR(RESET), argv0);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        return usage(argv[0]);
    int32_t arg = 1;
    // TODO opts
    error *er = core_main(argv[arg]);
    if (er) {
        error_print(er, stdout, 0, ERROR_PRINT(NL_END));
        error_free(er);
        return 1;
    }
    return 0;
}
