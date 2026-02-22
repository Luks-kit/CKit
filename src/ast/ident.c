#include "ident.h"
#include "eval_context.h"
#include "env.h"
#include <stdio.h>


Value identifier_eval(ASTBase* base, EvalContext* ctx)
{
    Identifier* id = (Identifier*)base;
    return env_lookup(ctx->current_env, id->name, id->name_length);
}

void identifier_print(ASTBase* base, EvalContext* ctx)
{
    Identifier* id = (Identifier*)base;
    printf("%.*s", (int)id->name_length, id->name);
}
