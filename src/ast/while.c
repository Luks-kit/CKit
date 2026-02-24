#include "while.h"
#include "ast_runtime.h"
#include <stdlib.h>
#include <stdio.h>



Value while_eval(ASTBase* base, EvalContext* ctx) {
    WhileStmt* node = (WhileStmt*)base;
    Value last_val = {.type = VAL_NULL};

    for (;;) {
        Value cond = ast_eval(node->condition, ctx);
        if (ctx->has_error || cond.type != VAL_BOOL || !cond.b) break;
        
        last_val = ast_eval(node->body, ctx);
    }

    return last_val;
}

void while_print(ASTBase* base, EvalContext* ctx) {
    WhileStmt* node = (WhileStmt*)base;
    printf("while (");
    ast_print(node->condition, ctx);
    printf(") ");
    ast_print(node->body, ctx);
}

ASTBase* while_new(ASTBase* cond, ASTBase* body) {
    WhileStmt* w = malloc(sizeof(WhileStmt));
    w->base.type = AST_WHILE;
    w->condition = cond;
    w->body = body;
    return (ASTBase*)w;
}

void while_destroy(ASTBase* node, EvalContext* ctx) {
    WhileStmt* w = (WhileStmt*)node;
    ast_dtor(w->condition, ctx);
    ast_dtor(w->body, ctx);
    free(w);
}
