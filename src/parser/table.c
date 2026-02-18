
#include "./table.h"

static error *parser_error(ast_container *cont, const char *msg) {
    return ERROR_INIT(0, &token_state_fn_table, def_ptr(&cont->state), msg);
}

static error *parser_next_stmt(ast_container *cont, ast_node **current_node, type_table *scope) {
    token_slice slice = { .class = TOKEN_CLASS(INVALID) };
    for (;;) {
        if (token_next(&cont->state, &slice) != DEF_STATUS(OK))
            return parser_error(cont, "invalid token next stmt");
        const parser_table_action *action = parser_table_action_next(slice.class);
        if (action->mode == PARSER_MODE(END_STMT) || action->mode == PARSER_MODE(END))
            break;
        ast_node *next_node = nullptr;
        switch (action->mode) {
            case PARSER_MODE(SKIP):
                continue;
            case PARSER_MODE(NEXT_STMT):
                if (!(next_node = action->ast_node_int_fn(&slice)))
                    return parser_error(cont, "invalid node for next stmt stmt");
                if (action->ast_node_join_fn(current_node, next_node) != DEF_STATUS(OK))
                    return parser_error(cont, "invalid node join for stmt");
                break;
            case PARSER_MODE(NEXT_LAMBDA):
                (void) scope;
                // TODO lambda
                return parser_error(cont, "TODO LAMBDA stmt");
                break;
            case PARSER_MODE(NEXT_APPLY):
                // TODO apply
                return parser_error(cont, "TODO APPLY stmt");
                break;
            case PARSER_MODE(NEXT_DEFINE):
                // TODO define
                return parser_error(cont, "TODO DEFINE stmt");
                break;
            default:
                return parser_error(cont, "invalid parser mode stmt");
        }
    }
    while ((*current_node)->parent)
        *current_node = (*current_node)->parent;
    return nullptr;
}

error *parser_next_list(ast_container *cont, ast_node *list_node, type_table *scope, parser_mode stop_mode) {
    token_slice slice = { .class = TOKEN_CLASS(INVALID) };
    for (;;) {
        if (token_next(&cont->state, &slice) != DEF_STATUS(OK))
            return parser_error(cont, "invalid token next list");
        const parser_table_action *action = parser_table_action_next(slice.class);
        if (action->mode == stop_mode)
            break;
        error *er = nullptr;
        ast_node *next_node = nullptr;
        switch (action->mode) {
            case PARSER_MODE(SKIP):
                continue;
            case PARSER_MODE(NEXT_STMT):
                if (!(next_node = action->ast_node_int_fn(&slice)))
                    return parser_error(cont, "invalid node for next stmt list");
                if ((er = parser_next_stmt(cont, &next_node, scope)))
                    return er;
                break;
            case PARSER_MODE(END_STMT):
                break;
            case PARSER_MODE(NEXT_LAMBDA):
                // TODO lambda
                return parser_error(cont, "TODO LAMBDA list");
                break;
            case PARSER_MODE(NEXT_APPLY):
                // TODO apply
                return parser_error(cont, "TODO APPLY list");
                break;
            case PARSER_MODE(NEXT_DEFINE):
                // TODO define
                return parser_error(cont, "TODO DEFINE list");
                break;
            default:
                return parser_error(cont, "invalid parser mode list");
        }
        if (ast_node_list_add_back(list_node, next_node) != DEF_STATUS(OK)) {
            if (next_node)
                ast_node_free(next_node);
            return ERROR_INIT(AST_NODE_PRINT(NL_END) | AST_NODE_PRINT(STRING) | AST_NODE_PRINT(POSITION),
                    &ast_node_fn_table, def_ptr(list_node), "invalid node list add back");
        }
        if (action->mode == PARSER_MODE(NEXT_STMT) && cont->state.line_no < 0)
            break;
    }
    return nullptr;
}

#define TABLE_MODE(CLASS, MODE, INT, JOIN) [TOKEN_CLASS(CLASS)] = (parser_table_action) { \
    .mode = PARSER_MODE(MODE), \
    .ast_node_int_fn = INT, \
    .ast_node_join_fn = JOIN }

static const parser_table_action table_action[TOKEN_CLASS(_)] = {
    TABLE_MODE(INVALID, INVALID, nullptr, nullptr),
    TABLE_MODE(END, END, nullptr, nullptr),
    TABLE_MODE(SPACES, SKIP, nullptr, nullptr),
    TABLE_MODE(TABS, SKIP, nullptr, nullptr),
    TABLE_MODE(NEW_LINE, END_STMT, nullptr, nullptr),
    TABLE_MODE(SEMICOLON, END_STMT, nullptr, nullptr),
    TABLE_MODE(TYPE, NEXT_STMT, parser_ast_node_init_type, parser_ast_node_join_value),
    TABLE_MODE(VAR, NEXT_STMT, parser_ast_node_init_var, parser_ast_node_join_value),
    TABLE_MODE(UNUSED_VAR, NEXT_STMT, parser_ast_node_init_var_unused, parser_ast_node_join_value),
    // TODO BASE
    TABLE_MODE(TRUE_VALUE, NEXT_STMT, parser_ast_node_init_true_value, parser_ast_node_join_value),
    TABLE_MODE(TRUE_CONST, NEXT_STMT, parser_ast_node_init_true_const, parser_ast_node_join_value),
    TABLE_MODE(FALSE_VALUE, NEXT_STMT, parser_ast_node_init_false_value, parser_ast_node_join_value),
    TABLE_MODE(FALSE_CONST, NEXT_STMT, parser_ast_node_init_false_const, parser_ast_node_join_value),
    TABLE_MODE(INTEGER, NEXT_STMT, parser_ast_node_init_integer, parser_ast_node_join_value),
    TABLE_MODE(FLOAT, NEXT_STMT, parser_ast_node_init_float, parser_ast_node_join_value),
    TABLE_MODE(STRING, NEXT_STMT, parser_ast_node_init_string, parser_ast_node_join_value),
    TABLE_MODE(SYMBOL, NEXT_STMT, parser_ast_node_init_symbol, parser_ast_node_join_value),
    TABLE_MODE(UNUSED_SYMBOL, NEXT_STMT, parser_ast_node_init_symbol_unused, parser_ast_node_join_value),
    TABLE_MODE(COMMAND, NEXT_STMT, parser_ast_node_init_command, parser_ast_node_join_value),
    TABLE_MODE(APPLY_START, NEXT_APPLY, nullptr, nullptr),
    TABLE_MODE(APPLY_END, END_APPLY, nullptr, nullptr),
    TABLE_MODE(DEFINE_START, NEXT_DEFINE, nullptr, nullptr),
    TABLE_MODE(DEFINE_END, END_DEFINE, nullptr, nullptr),
    TABLE_MODE(LAMBDA_START, NEXT_LAMBDA, nullptr, nullptr),
    TABLE_MODE(LAMBDA_END, END_LAMBDA, nullptr, nullptr),
    TABLE_MODE(TILDE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(EXCLAMATION, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(EXCLAMATION_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(EXCLAMATION_EQUAL, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(EXCLAMATION_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(EXCLAMATION_DOUBLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(POUND, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(AT, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(DOLLAR, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PERCENT, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PERCENT_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(CARET, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(CARET_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(CARET_DOUBLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(AMPERSAND, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(AMPERSAND_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(AMPERSAND_DOUBLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(STAR, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(STAR_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(STAR_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(STAR_DOUBLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(DASH, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(DASH_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PLUS, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PLUS_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(EQUAL, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(EQUAL_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PIPE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PIPE_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PIPE_DOUBLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(COLON_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(QUOTE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT_COLON_ARROW_RIGHT, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT_EQUAL, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT_EQUAL_ARROW_RIGHT, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT_DOUBLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT_TRIPLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_LEFT_TRIPLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(COMMA, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(COMMA_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_RIGHT, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_RIGHT_EQUAL, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_RIGHT_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_RIGHT_DOUBLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_RIGHT_TRIPLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(ARROW_RIGHT_TRIPLE_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(PERIOD_DOUBLE, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(QUESTION, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(FOWARD_SLASH, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(FOWARD_SLASH_COLON, NEXT_STMT, parser_ast_node_init_op, parser_ast_node_join_op),
    TABLE_MODE(COMMENT_RANGE, SKIP, nullptr, nullptr),
    TABLE_MODE(COMMENT_LINE, SKIP, nullptr, nullptr)
};

const parser_table_action *parser_table_action_next(token_class class) {
    if (class >= TOKEN_CLASS(_))
        return &table_action[TOKEN_CLASS(INVALID)];
    return &table_action[class];
}
