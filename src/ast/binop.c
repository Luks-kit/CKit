#include "binop.h"
#include <stdio.h>
#include <stdlib.h>


static double to_double(Value v)
{
    if (v.type == VAL_FLOAT) return v.f;
    if (v.type == VAL_INT)   return (double)v.i;
    return 0.0;
}

Value binop_eval(ASTBase* base, EvalContext* ctx)
{
    BinOp* bin = (BinOp*)base;
    Value left  = ast_eval(bin->lhs, ctx);
    Value right = ast_eval(bin->rhs, ctx);

    if (ctx->has_error) return (Value){ .type = VAL_NULL };

    if (left.type == VAL_INT && right.type == VAL_INT) {
        switch (bin->op) {
            case '+': return (Value){ VAL_INT, .i = left.i + right.i };
            case '-': return (Value){ VAL_INT, .i = left.i - right.i };
            case '*': return (Value){ VAL_INT, .i = left.i * right.i };
            case '/': return (Value){ VAL_INT, .i = left.i / right.i };
        }
    }

    double l = to_double(left);
    double r = to_double(right);

    switch (bin->op) {
        case '+': return (Value){ VAL_FLOAT, .f = l + r };
        case '-': return (Value){ VAL_FLOAT, .f = l - r };
        case '*': return (Value){ VAL_FLOAT, .f = l * r };
        case '/': return (Value){ VAL_FLOAT, .f = l / r };
    }

    ctx->has_error = 1;
    ctx->error_message = "Invalid binary operation";
    return (Value){ .type = VAL_NULL };
}

void binop_print(ASTBase* base, EvalContext* ctx)
{
    BinOp* bin = (BinOp*)base;
    printf("(");
    ast_print(bin->lhs, ctx);
    printf(" %c ", bin->op);
    ast_print(bin->rhs, ctx);
    printf(")");
}


ASTBase* binop_new(char op, ASTBase* lhs, ASTBase* rhs) {
    BinOp* bin = malloc(sizeof(BinOp));
    bin->base.type = AST_BINOP;
    bin->op = op;
    bin->lhs = lhs;
    bin->rhs = rhs;
    return &bin->base;
}

void binop_destroy(ASTBase* node, EvalContext* ctx) {
    BinOp* bin = (BinOp*)node;
    ast_dtor(bin->lhs, ctx);
    ast_dtor(bin->rhs, ctx);
    free(bin);
}
