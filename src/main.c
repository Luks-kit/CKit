#include <stdio.h>
#include <stdlib.h>
#include "ast_runtime.h"
#include "eval_context.h"
#include "literal.h"
#include "ident.h"
#include "binop.h"
#include "unop.h"
#include "assign.h"
#include "block.h"
#include "env.h"

int main(void)
{
    Runtime rt;
    runtime_init(&rt);
    ast_register_all(&rt);

    EvalContext ctx = {0};
    ctx.current_env = env_push(NULL);
    ctx.runtime = &rt;

    /* x = 10 */
    IntLiteral* ten = malloc(sizeof(IntLiteral));
    ten->literal.base.type = AST_LITERAL;
    ten->literal.lit_type = AST_LIT_INT;
    ten->value = 10;

    Identifier* x = malloc(sizeof(Identifier));
    x->base.type = AST_IDENTIFIER;
    x->name = "x";
    x->name_length = 1;

    Assign* assign_x = malloc(sizeof(Assign));
    assign_x->base.type = AST_ASSIGN;
    assign_x->target = x;
    assign_x->value = &(ten->literal.base);

    /* y = x + 20 */
    IntLiteral* twenty = malloc(sizeof(IntLiteral));
    twenty->literal.base.type = AST_LITERAL;
    twenty->literal.lit_type = AST_LIT_INT;
    twenty->value = 20;

    Identifier* y = malloc(sizeof(Identifier));
    y->base.type = AST_IDENTIFIER;
    y->name = "y";
    y->name_length = 1;

    BinOp* add = malloc(sizeof(BinOp));
    add->base.type = AST_BINOP;
    add->op = '+';
    add->lhs = &x->base;
    add->rhs = &(twenty->literal.base);

    Assign* assign_y = malloc(sizeof(Assign));
    assign_y->base.type = AST_ASSIGN;
    assign_y->target = y;
    assign_y->value = &add->base;

    /* Block: { x = 10; y = x + 20; } */
    ASTBase* statements[] = {&assign_x->base, &assign_y->base};
    Block* block = malloc(sizeof(Block));
    block->base.type = AST_BLOCK;
    block->statements = statements;
    block->count = 2;

    Value result = ast_eval(&(block->base), &ctx);
    ast_print(&(block->base), &ctx);
    printf("\nResult type: %d\n", result.type);
    printf("Result: y = %ld\n", result.i);

    env_pop(ctx.current_env);
    return 0;
}
