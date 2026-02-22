#include "ast_runtime.h"
#include "literal.h"
#include "binop.h"
#include "unop.h"
#include "ident.h"
#include "assign.h"
#include "block.h"
#include "call.h"

void ast_register_all(Runtime* rt)
{
    ast_register(rt, AST_LITERAL,  NULL, NULL, literal_eval, literal_print);
    ast_register(rt, AST_IDENTIFIER, NULL, NULL, identifier_eval, identifier_print);
    ast_register(rt, AST_BINOP, NULL, NULL, binop_eval, binop_print);
    ast_register(rt, AST_UNOP, NULL, NULL, unop_eval, unop_print);
    ast_register(rt, AST_CALL, NULL, NULL, call_eval, call_print);
    ast_register(rt, AST_ASSIGN, NULL, NULL, assign_eval, assign_print);
    ast_register(rt, AST_BLOCK,  NULL, NULL, block_eval,  block_print); 
}
