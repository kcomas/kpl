
#include "./util.h"

string *namespace_util_file_abs_path(const char *resolvepath, const char *filepath) {
    char path[PATH_MAX] = {};
    size_t path_len = 0;
    if (filepath[0] == '/')
        return string_init_c_str(filepath);
    if (resolvepath) {
        strncpy(path, resolvepath, PATH_MAX - 1);
        dirname(path);
    } else if (!getcwd(path, PATH_MAX))
        return NULL;
    path_len = strlen(path);
    path[path_len++] = '/';
    strncpy(path + path_len, filepath, PATH_MAX - path_len - 1);
    string *st = string_init(strlen(path));
    if (!realpath(path, st->data) && errno != ENOENT) {
        string_free(st);
        return NULL;
    }
    st->len = strlen(st->data);
    return st;
}
