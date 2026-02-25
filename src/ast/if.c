#include <stdlib.h>
#include <stdio.h>
#include "if.h"
#include "ast_runtime.h"
#include "value.h"

ASTBase* if_new(ASTBase* cond, ASTBase* then_b, ASTBase* else_b) {
    IfStmt* i = malloc(sizeof(IfStmt));
    i->base.type = AST_IF;
    i->condition = cond;
    i->then_branch = then_b;
    i->else_branch = else_b;
    return (ASTBase*)i;
}

void if_destroy(ASTBase* node, EvalContext* ctx) {
    IfStmt* i = (IfStmt*)node;
    ast_dtor(i->condition, ctx);
    ast_dtor(i->then_branch, ctx);
    if (i->else_branch) ast_dtor(i->else_branch, ctx);
    free(i);
}


void if_print(ASTBase* base, EvalContext* ctx) {
    IfStmt* node = (IfStmt*)base;
    printf("if (");
    ast_print(node->condition, ctx);
    printf(") ");
    ast_print(node->then_branch, ctx);
    if (node->else_branch) {
        printf(" else ");
        ast_print(node->else_branch, ctx);
    }
}

Value if_eval(ASTBase* node, EvalContext* ctx) {
    IfStmt* i = (IfStmt*)node;
    // Condition is guaranteed to be VAL_BOOL
    if (ast_eval(i->condition, ctx).b) {
        return ast_eval(i->then_branch, ctx);
    } else if (i->else_branch) {
        return ast_eval(i->else_branch, ctx);
    }
    return (Value){.type = VAL_NULL};
}

ValueType if_get_type(ASTBase* node, EvalContext* ctx) {
    IfStmt* i = (IfStmt*)node;
    if (!i->else_branch) return VAL_NULL;

    ValueType then_t = ast_get_type(i->then_branch, ctx);
    ValueType else_t = ast_get_type(i->else_branch, ctx);

    if (then_t == else_t) return then_t;
    return VAL_NULL; // Or throw a type error in validate
}

bool if_validate(ASTBase* node, EvalContext* ctx) {
    IfStmt* i = (IfStmt*)node;
    
    if (!ast_validate(i->condition, ctx)) return false;
    if (ast_get_type(i->condition, ctx) != VAL_BOOL) {
        fprintf(stderr, "Error: If condition must be bool.\n");
        ctx->has_error = true; return false;
    }

    if (!ast_validate(i->then_branch, ctx)) return false;
    
    if (i->else_branch) {
        if (!ast_validate(i->else_branch, ctx)) return false;
        
        // Rust-style: Branches must match types if used as expression
        if (ast_get_type(i->then_branch, ctx) != ast_get_type(i->else_branch, ctx)) {
            fprintf(stderr, "Error: If/Else branch type mismatch.\n");
            ctx->has_error = true; return false;
        }
    }

    return true;
}
