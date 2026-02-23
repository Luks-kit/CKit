#include "ident.h"
#include "eval_context.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

ASTBase* identifier_new(const char* name, size_t length) {
    Identifier* id = malloc(sizeof(Identifier));
    id->base.type = AST_IDENTIFIER;

    char* copy = malloc(length);
    memcpy(copy, name, length);

    id->name = copy;
    id->name_length = length;

    return &id->base;
}

void identifier_destroy(ASTBase* node, EvalContext* ctx) {
    Identifier* id = (Identifier*)node;
    free((void*)id->name);
    free(node);
}
