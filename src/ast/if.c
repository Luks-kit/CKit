#include <stdlib.h>
#include <stdio.h>
#include "if.h"
#include "ast_runtime.h"

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

Value if_eval(ASTBase* base, EvalContext* ctx) {
    IfStmt* node = (IfStmt*)base;
    Value cond = ast_eval(node->condition, ctx);
    
    // Strong typing check: condition MUST be a boolean
if (cond.type != VAL_BOOL) {
        fprintf(stderr, "Runtime Error: 'if' condition must be a boolean.\n");
        ctx->has_error = true;
        return (Value){.type = VAL_NULL};
    }

    if (cond.b) {
        return ast_eval(node->then_branch, ctx);
    } else if (node->else_branch) {
        return ast_eval(node->else_branch, ctx);
    }

    return (Value){.type = VAL_NULL};
}
