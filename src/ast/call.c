#include "call.h"
#include <stdio.h>

/* Minimal placeholder */
Value call_eval(ASTBase* base, EvalContext* ctx)
{
    Call* call = (Call*)base;
    ctx->has_error = 1;
    ctx->error_message = "Function calls not implemented yet";
    return (Value){ .type = VAL_NULL };
}

void call_print(ASTBase* base, EvalContext* ctx)
{
    Call* call = (Call*)base;
    printf("%s(", call->name);
    for (size_t i = 0; i < call->arg_count; i++) {
        ast_print(call->args[i], ctx);
        if (i + 1 < call->arg_count) printf(", ");
    }
    printf(")");
}
