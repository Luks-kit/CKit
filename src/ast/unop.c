#include "unop.h"
#include <stdio.h>

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

void unop_print(ASTBase* base, EvalContext* ctx)
{
    UnOp* un = (UnOp*)base;
    printf("%c", un->op);
    ast_print(un->operand, ctx);
}
