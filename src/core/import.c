
#include "./import.h"

void core_import_before(core_queue_item *parent) {
    parent->state = QUEUE_ITEM_STATE(DEPENDENCIES);
    parent->dependencies++;
    parent->queue->state_count.init--;
    parent->queue->state_count.dependencies++;
}

static error *import_string_slice(core_queue_item *parent, uint32_t start_idx, uint32_t end_idx) {
    char import_string[PATH_MAX] = {};
    memcpy(import_string, parent->filedata->data + start_idx, end_idx - start_idx);
    core_queue_item *dependent = core_queue_add(parent->queue, parent->filename->data, import_string);
    if (!dependent)
        return core_queue_item_error(parent, "Import Add Failed");
    core_queue_item_add_parent(dependent, parent);
    return NULL;
}

static const char *import_match = "\\import";

static const uint32_t import_match_len = 8;

static const uint32_t min_import_len = 2;

error *core_import(core_queue_item *parent) {
    if (!(parent->filedata = core_util_file_read_string(parent->filename->data)))
        return core_queue_item_error(parent, "Failed To Read File");
    uint32_t filedata_idx = 0;
    while (filedata_idx < parent->filedata->len) {
        uint32_t import_match_idx = 0;
        while (filedata_idx < parent->filedata->len && import_match[import_match_idx] &&
            parent->filedata->data[filedata_idx] == import_match[import_match_idx]) {
            filedata_idx++;
            import_match_idx++;
        }
        if (import_match[import_match_idx] || isalnum(parent->filedata->data[filedata_idx])) {
            filedata_idx++;
            continue;
        }
        uint32_t start_idx = 0, end_idx = filedata_idx - import_match_len;
        while (parent->filedata->data[end_idx] == ' ')
            end_idx--;
        while (parent->filedata->data[end_idx] != '"')
            end_idx--;
        if (end_idx < min_import_len)
            return core_queue_item_error(parent, "Invalid Import Format");
        start_idx = end_idx - 1;
        while (start_idx > 0) {
            if (parent->filedata->data[start_idx] == '"' &&
                (start_idx == 0 || parent->filedata->data[start_idx - 1] != '\\'))
                break;
            start_idx--;
        }
        if (parent->filedata->data[start_idx] != '"')
            return core_queue_item_error(parent, "Invalid Import Format");
        start_idx++;
        error *er = import_string_slice(parent, start_idx, end_idx);
        if (er)
            return er;
    }
    parent->dependencies--;
    return NULL;
}
