#include "block.h"
#include "eval_context.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>

Value block_eval(ASTBase* base, EvalContext* ctx)
{
    Block* block = (Block*)base;

    /* Push new lexical scope */
    Env* old_env = ctx->current_env;
    ctx->current_env = env_push(old_env);

    Value result = { .type = VAL_NULL };

    for (size_t i = 0; i < block->count; i++) {
        result = ast_eval(block->statements[i], ctx);
        if (ctx->has_error) break;
    }

    /* Pop lexical scope */
    ctx->current_env = env_pop(ctx->current_env);

    return result;
}

void block_print(ASTBase* base, EvalContext* ctx)
{
    Block* block = (Block*)base;
    printf("{\n");
    for (size_t i = 0; i < block->count; i++) {
        ast_print(block->statements[i], ctx);
        printf(";\n");
    }
    printf("}");
}
