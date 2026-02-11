
#include "./node_type.h"

static const char *node_type_strs[] = {
    "ATOM",
    "LIST",
    "OP"
};

const char *ast_node_type_str(ast_node_type node_type) {
    if (node_type >= AST_NODE_TYPE(_))
        return "INVALID AST NODE TYPE";
    return node_type_strs[node_type];
}

#define AST_NAME(AST, TYPE) [TYPE_NAME(TYPE)] = AST_NODE_TYPE(AST)

static const ast_node_type type_name_node_type_array[] = {
    // SCALAR
    AST_NAME(ATOM, VOID),
    AST_NAME(ATOM, BOOL),
    AST_NAME(ATOM, U8),
    AST_NAME(ATOM, U16),
    AST_NAME(ATOM, U32),
    AST_NAME(ATOM, U64),
    AST_NAME(ATOM, I8),
    AST_NAME(ATOM, I16),
    AST_NAME(ATOM, I32),
    AST_NAME(ATOM, I64),
    AST_NAME(ATOM, F32),
    AST_NAME(ATOM, F64),
    AST_NAME(ATOM, CHAR),
    AST_NAME(ATOM, STRING),
    AST_NAME(ATOM, ERROR),
    AST_NAME(ATOM, BUFFER),
    AST_NAME(ATOM, FD),
    AST_NAME(ATOM, SOCKET),
    // SIZE
    AST_NAME(LIST, ARRAY),
    AST_NAME(ATOM, TMP),
    // VECTOR
    AST_NAME(LIST, VECTOR),
    AST_NAME(LIST, SET),
    AST_NAME(ATOM, RESULT),
    AST_NAME(ATOM, OPTION),
    AST_NAME(ATOM, THREAD),
    // LIST
    AST_NAME(OP, OP),
    AST_NAME(LIST, TUPLE),
    AST_NAME(LIST, MAP),
    AST_NAME(LIST, OVERLOAD),
    AST_NAME(ATOM, CLOSURE),
    AST_NAME(LIST, APPLY),
    // TABLE
    AST_NAME(LIST, ENUM),
    AST_NAME(LIST, STRUCT),
    AST_NAME(LIST, UNION),
    AST_NAME(LIST, FN),
    AST_NAME(LIST, COROUTINE),
    AST_NAME(LIST, LAMBDA),
    AST_NAME(ATOM, REGEX),
    AST_NAME(ATOM, RANGE),
    AST_NAME(LIST, DEFINE),
    // TAG
    AST_NAME(ATOM, TAG),
    AST_NAME(ATOM, VALUE),
    AST_NAME(ATOM, ALIAS),
    AST_NAME(ATOM, VAR),
    AST_NAME(ATOM, ARG),
    // BASE
    AST_NAME(LIST, BASE),
    AST_NAME(_, _)
};

ast_node_type ast_node_get_ast_type(const type *ty) {
    if (!ty)
        return AST_NODE_TYPE(_);
    if (ty->name >= TYPE_NAME(_))
        return AST_NODE_TYPE(_);
    return type_name_node_type_array[ty->name];
}
