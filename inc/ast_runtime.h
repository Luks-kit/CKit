#ifndef AST_RUNTIME_H
#define AST_RUNTIME_H

#include "ast.h"
#include "value.h"
#include "eval_context.h"

typedef void   (*AstCtorFn)(ASTBase* node, EvalContext* ctx);
typedef void   (*AstDtorFn)(ASTBase* node, EvalContext* ctx);
typedef Value  (*AstEvalFn)(ASTBase* node, EvalContext* ctx);
typedef void   (*AstPrintFn)(ASTBase* node, EvalContext* ctx);

/* ================================
   Runtime Struct
   ================================ */

typedef struct Runtime {
    AstCtorFn  ctor_table[AST_TYPE_COUNT];
    AstDtorFn  dtor_table[AST_TYPE_COUNT];
    AstEvalFn  eval_table[AST_TYPE_COUNT];
    AstPrintFn print_table[AST_TYPE_COUNT];
} Runtime;

/* ================================
   Dispatch Helpers
   ================================ */

static inline void ast_ctor(ASTBase* node, EvalContext* ctx) {
    ctx->runtime->ctor_table[node->type](node, ctx);
}

static inline void ast_dtor(ASTBase* node, EvalContext* ctx) {
    ctx->runtime->dtor_table[node->type](node, ctx);
}

static inline Value ast_eval(ASTBase* node, EvalContext* ctx) {
    return ctx->runtime->eval_table[node->type](node, ctx);
}

static inline void ast_print(ASTBase* node, EvalContext* ctx) {
    ctx->runtime->print_table[node->type](node, ctx);
}

/* ================================
   Registration API
   ================================ */

void ast_register(Runtime* rt,
                  AST_TYPE type,
                  AstCtorFn ctor,
                  AstDtorFn dtor,
                  AstEvalFn eval,
                  AstPrintFn print);

/* Initialize tables to defaults */
void runtime_init(Runtime* rt);
void ast_register_all(Runtime* rt);

#endif
