#include "ast.h"
#include "ast_runtime.h"
#include "decl.h"
#include "if.h"
#include "literal.h"
#include "binop.h"
#include "unop.h"
#include "ident.h"
#include "assign.h"
#include "block.h"
#include "call.h"
#include "while.h"
#include "fndecl.h"

void ast_register_all(Runtime* rt)
{
    ast_register(rt, AST_LITERAL,  literal_validate, literal_destroy, literal_eval, literal_print, literal_get_type);
    ast_register(rt, AST_IDENTIFIER, identifier_validate, identifier_destroy, identifier_eval, identifier_print, identifier_get_type);
    ast_register(rt, AST_BINOP, binop_validate, binop_destroy, binop_eval, binop_print, binop_get_type);
    ast_register(rt, AST_UNOP, unop_validate, unop_destroy, unop_eval, unop_print, unop_get_type);
    ast_register(rt, AST_CALL, call_validate, call_destroy, call_eval, call_print, call_get_type);
    ast_register(rt, AST_ASSIGN, assign_validate, assign_destroy, assign_eval, assign_print, assign_get_type);
    ast_register(rt, AST_BLOCK,  block_validate, block_destroy, block_eval, block_print, block_get_type);
    ast_register(rt, AST_IF, if_validate, if_destroy, if_eval, if_print, if_get_type);
    ast_register(rt, AST_WHILE, while_validate, while_destroy, while_eval, while_print, while_get_type);
    ast_register(rt, AST_VAR_DECL, decl_validate, decl_destroy, decl_eval, decl_print, decl_get_type);
    ast_register(rt, AST_FUNC_DECL, fndecl_validate, fndecl_destroy, fndecl_eval, fndecl_print, fndecl_get_type);
}
