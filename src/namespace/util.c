
#include "./util.h"

string *namespace_util_file_abs_path(const char *resolvepath, const char *filepath) {
    char path[PATH_MAX] = {};
    size_t path_len = 0;
    if (filepath[0] != '/') {
        if (resolvepath) {
            strncpy(path, resolvepath, PATH_MAX - 1);
            dirname(path);
        } else if (!getcwd(path, PATH_MAX))
            return NULL;
        path_len = strlen(path);
        path[path_len++] = '/';
    }
    strncpy(path + path_len, filepath, PATH_MAX - path_len - 1);
    string *st = string_init(strlen(path));
    if (!realpath(path, st->data)) {
        string_free(st);
        return NULL;
    }
    st->len = strlen(st->data);
    return st;
}

string *namespace_util_file_read_string(const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1)
        return NULL;
    struct statx stx_buf;
    if (statx(fd, NULL, AT_EMPTY_PATH, STATX_SIZE, &stx_buf) == -1) {
        close(fd);
        return NULL;
    }
    string *st = string_init(stx_buf.stx_size);
    st->len = stx_buf.stx_size;
    if (read(fd, st->data, stx_buf.stx_size) != (ssize_t) stx_buf.stx_size) {
        close(fd);
        string_free(st);
        return NULL;
    }
    close(fd);
    return st;
}
