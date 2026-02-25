#include "decl.h"
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "ast_runtime.h"
#include "env.h"

Value decl_eval(ASTBase* base, EvalContext* ctx) {
    Declaration* decl = (Declaration*)base;
    Value val = { .type = VAL_NULL };

    if (decl->initializer) {
        val = ast_eval(decl->initializer, ctx);
        
        // TYPE CHECK:
        if (val.type != decl->type) {
            fprintf(stderr, "Runtime TypeError: Cannot assign %s to variable of type %s\n",
                    type_to_string(val.type), type_to_string(decl->type));
            ctx->has_error = true;
            return (Value){ .type = VAL_NULL};
        }
    } else {
        // Provide "Zero" values for strong typing
        val.type = decl->type;
        if (val.type == VAL_INT) val.i = 0;
        else if (val.type == VAL_BOOL) val.b = false;
        else if (val.type == VAL_STRING) { val.s.data = NULL; val.s.length = 0; }
    }

    env_set(ctx->current_env, decl->name, decl->name_len, val);
    return val;
}

ValueType decl_get_type(ASTBase* node, EvalContext* ctx) {
    return ((Declaration*)node)->type;
}

bool decl_validate(ASTBase* node, EvalContext* ctx) {
    Declaration* decl = (Declaration*)node;
    if (decl->initializer) {
        if (!ast_validate(decl->initializer, ctx)) return false;
        if (ast_get_type(decl->initializer, ctx) != decl->type) {
            fprintf(stderr, "Type Error: Initializer mismatch for '%.*s'.\n", (int)decl->name_len, decl->name);
            ctx->has_error = true; return false;
        }
    }
    // Register the variable in the env so future lines can validate against it
    Value placeholder = { .type = decl->type };
    env_set(ctx->current_env, decl->name, decl->name_len, placeholder);
    return true;
}

void decl_print(ASTBase* base, EvalContext* ctx) {
    Declaration* decl = (Declaration*)base;
    // Map your ValueType enum back to strings
    printf("%s %.*s", type_to_string(decl->type), (int)decl->name_len, decl->name);
    if (decl->initializer) {
        printf(" = ");
        ast_print(decl->initializer, ctx);
    }
}

ASTBase* decl_new(ValueType type, const char* name, size_t len, ASTBase* init) {
    Declaration* d = malloc(sizeof(Declaration));
    d->base.type = AST_VAR_DECL;
    d->type = type;
    // We store the pointer, but remember: env_set will copy it later.
    // If you want the AST to own the string, strndup it here.
    d->name = name; 
    d->name_len = len;
    d->initializer = init;
    return &d->base;
}

void decl_destroy(ASTBase* node, EvalContext* ctx) {
    Declaration* d = (Declaration*)node;
    if (d->initializer) ast_dtor(d->initializer, ctx);
    free(d);
}


