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
    if(!block->is_global) ctx->current_env = env_push(old_env);

    Value result = { .type = VAL_NULL };

    for (size_t i = 0; i < block->count; i++) {
        result = ast_eval(block->statements[i], ctx);
        if (ctx->has_error) break;
    }

    /* Pop lexical scope */
    if(!block->is_global) ctx->current_env = env_pop(ctx->current_env);

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

ASTBase* block_new(ASTBase** statements, size_t count, bool isGlobal) {
    Block* block = malloc(sizeof(Block));
    block->base.type = AST_BLOCK;
    block->count = count;	
    block->is_global = isGlobal;

    if (count == 0) {block->statements = NULL;}
    else {
    	block->statements = malloc(sizeof(ASTBase*) * count);
    	for (size_t i = 0; i < count; i++)
            block->statements[i] = statements[i];
    }

    return &block->base;
}

void block_destroy(ASTBase* node, EvalContext* ctx) {
    Block* block = (Block*)node;

    for (size_t i = 0; i < block->count; i++)
        ast_dtor(block->statements[i], ctx);

    free(block->statements);
    free(block);
}
