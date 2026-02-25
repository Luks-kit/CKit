#include "fndecl.h"
#include "env.h"
#include "value.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

ASTBase* fndecl_new(const char* name, size_t name_len, Parameter* params, size_t param_count, ASTBase* body) {
    FnDecl* node = malloc(sizeof(FnDecl));
    
    node->base.type = AST_FUNC_DECL;
    node->name = strndup(name, name_len);
    node->name_len = name_len;
    node->params = params; // Assumes caller allocated this array
    node->param_count = param_count;
    node->body = body;
    
    return (ASTBase*)node;
}

void fndecl_destroy(ASTBase* node, EvalContext* ctx) {
    FnDecl* decl = (FnDecl*)node;
    decl->params = NULL;
    decl->name = NULL;
    decl->body = NULL;
    free(decl);
}

void fndecl_print(ASTBase* node, EvalContext* ctx) {
    FnDecl* decl = (FnDecl*)node;
    printf("fn %.*s(", (int)decl->name_len, decl->name);
    for (size_t i = 0; i < decl->param_count; i++) {
        printf("%s %.*s", type_to_string(decl->params[i].type), 
               (int)decl->params[i].name_len, decl->params[i].name);
        if (i < decl->param_count - 1) printf(", ");
    }
    printf(") ");
    ast_print(decl->body, ctx);
}

bool fndecl_validate(ASTBase* node, EvalContext* ctx) {
    FnDecl* decl = (FnDecl*)node;

    // 1. Create the Function object
    Function* f = malloc(sizeof(Function));
    f->is_native = false;
    f->arity = (int)decl->param_count;
    f->name = decl->name;
    f->name_len = decl->name_len;
    f->user.body = decl->body;  // body can stay shared if AST lifetime > function lifetime

    f->user.param_count = decl->param_count;
    f->user.params = decl->params;

     
    // 2. Define the function in the current env (Global)
    Value f_val = { .type = VAL_FUNCTION, .function = f };
    env_set(ctx->current_env, f->name, f->name_len, f_val);

    // 3. Enter function scope to validate the body
    Env* outer = ctx->current_env;
    ctx->current_env = env_push(outer);

    for (size_t i = 0; i < f->user.param_count; i++) {
        Value param_placeholder = { .type = f->user.params[i].type };
        env_set(ctx->current_env, f->user.params[i].name, 
                   f->user.params[i].name_len, param_placeholder);
    }

    bool valid = ast_validate(f->user.body, ctx);
    
    // 4. Exit scope
    ctx->current_env = env_pop(ctx->current_env);
    return valid;
}

Value fndecl_eval(ASTBase* node, EvalContext* ctx) {
    // Already registered in validation pass.
    return (Value){.type = VAL_NULL}; 
}

ValueType fndecl_get_type(ASTBase* node, EvalContext* ctx) {
    return VAL_NULL;
}
