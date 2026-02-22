#include "assign.h"
#include "eval_context.h"
#include "env.h"
#include <stdio.h>

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
