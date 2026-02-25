#include "assign.h"
#include "eval_context.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>

Value assign_eval(ASTBase* node, EvalContext* ctx) {
    Assign* a = (Assign*)node;
    Value val = ast_eval(a->value, ctx);
    
    // env_assign is now trusted to find the variable in the parent chain
    env_assign(ctx->current_env, a->target->name, a->target->name_length, val);
    return val;
}

bool assign_validate(ASTBase* node, EvalContext* ctx) {
    Assign* a = (Assign*)node;
    if (!ast_validate(a->value, ctx)) return false;

    Value existing = env_lookup(ctx->current_env, a->target->name, a->target->name_length);
    if (existing.type == VAL_NULL) {
        ctx->has_error = true; return false;
    }

    Value new_val = ast_eval(a->value, ctx);
    if (existing.type != new_val.type) {
        fprintf(stderr, "Error: Cannot assign type %s to variable of type %s\n", 
                type_to_string(new_val.type), type_to_string(existing.type));
        ctx->has_error = true; return false;
    }
    return true;
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

ValueType assign_get_type(ASTBase* node, EvalContext* ctx) {
    return ast_get_type(((Assign*)node)->value, ctx);
}

void assign_destroy(ASTBase* node, EvalContext* ctx) {
    Assign* a = (Assign*)node;
    ast_dtor(&a->target->base, ctx);
    ast_dtor(a->value, ctx);
    free(a);
}
