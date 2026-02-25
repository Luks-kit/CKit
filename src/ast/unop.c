#include "unop.h"
#include "ast_runtime.h"
#include "value.h"
#include <stdio.h>
#include <stdlib.h>

Value unop_eval(ASTBase* base, EvalContext* ctx)
{
    UnOp* un = (UnOp*)base;
    Value v = ast_eval(un->operand, ctx);

    if (v.type == VAL_INT && un->op == '-') return (Value){ VAL_INT, .i = -v.i };
    if (v.type == VAL_FLOAT && un->op == '-') return (Value){ VAL_FLOAT, .f = -v.f };
    if (v.type == VAL_BOOL && un->op == '!') return (Value){ VAL_BOOL, .b = !v.b };

    ctx->has_error = 1;
    ctx->error_message = "Invalid unary operation";
    return (Value){ .type = VAL_NULL };
}

ValueType unop_get_type(ASTBase *base, EvalContext *ctx) {
    return ast_get_type( ((UnOp*)base)->operand, ctx);
}

bool unop_validate(ASTBase* node, EvalContext* ctx) {
    UnOp* un = (UnOp*)node;
    if (!ast_validate(un->operand, ctx)) return false;

    Value val = ast_eval(un->operand, ctx);
    if (un->op == '!' && val.type != VAL_BOOL) {
        fprintf(stderr, "Error: '!' requires boolean operand\n");
        ctx->has_error = true; return false;
    }
    return true;
}

void unop_print(ASTBase* base, EvalContext* ctx)
{
    UnOp* un = (UnOp*)base;
    printf("%c", un->op);
    ast_print(un->operand, ctx);
}

ASTBase* unop_new(char op, ASTBase* operand) {
    UnOp* un = malloc(sizeof(UnOp));
    un->op = op;
    un->operand = operand;
    return &un->base;

}

void unop_destroy(ASTBase* base, EvalContext* ctx) {
    UnOp* un = (UnOp*)base;
    ast_dtor(&un->base, ctx);
    free(un);
}
