#include "ident.h"
#include "eval_context.h"
#include "env.h"
#include "value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Value identifier_eval(ASTBase* base, EvalContext* ctx)
{
    Identifier* id = (Identifier*)base;
    return env_lookup(ctx->current_env, id->name, id->name_length);
}

bool identifier_validate(ASTBase* node, EvalContext* ctx) {
    if (identifier_get_type(node, ctx) == VAL_NULL) {
        fprintf(stderr, "Error: Symbol '%.*s' not found in current scope.\n", 
                (int)((Identifier*)node)->name_length, ((Identifier*)node)->name);
        ctx->has_error = true;
        return false;
    }
    return true;
}

ValueType identifier_get_type(ASTBase* node, EvalContext* ctx) {
    if(!node) return VAL_NULL;
    Identifier* id = (Identifier*)node;
    Value v = env_lookup(ctx->current_env, id->name, id->name_length);
    return v.type;
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
