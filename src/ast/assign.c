#include "assign.h"
#include "eval_context.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Value assign_eval(ASTBase* base, EvalContext* ctx)
{
    Assign* assign = (Assign*)base;
    Value val = ast_eval(assign->value, ctx);

    if (!ctx->has_error) {
        env_set(ctx->current_env,
                assign->target->name,
                assign->target->name_length,
                val);
    }

    return val;
}

void assign_print(ASTBase* base, EvalContext* ctx)
{
    Assign* assign = (Assign*)base;
    ast_print(&(assign->target->base), ctx);
    printf(" = ");
    ast_print(assign->value, ctx);
}

ASTBase* assign_new(ASTBase* target, ASTBase* value) {
    Assign* a = malloc(sizeof(Assign));
    a->base.type = AST_ASSIGN;
    a->target = (Identifier*)target;
    a->value = value;
    return &a->base;
}

void assign_destroy(ASTBase* node, EvalContext* ctx) {
    Assign* a = (Assign*)node;
    ast_dtor(&a->target->base, ctx);
    ast_dtor(&a->value, ctx);
    free(a);
}
