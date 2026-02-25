#ifndef AST_RUNTIME_H
#define AST_RUNTIME_H

#include "ast.h"
#include "value.h"
#include "eval_context.h"

typedef ValueType (*AstTypeFn)(ASTBase* node, EvalContext* ctx);
typedef bool   (*AstValidateFn)(ASTBase* node, EvalContext* ctx);
typedef void   (*AstDtorFn)(ASTBase* node, EvalContext* ctx);
typedef Value  (*AstEvalFn)(ASTBase* node, EvalContext* ctx);
typedef void   (*AstPrintFn)(ASTBase* node, EvalContext* ctx);

/* ================================
   Runtime Struct
   ================================ */

typedef struct Runtime {
    AstValidateFn  validate_table [AST_TYPE_COUNT];
    AstDtorFn      dtor_table     [AST_TYPE_COUNT];
    AstEvalFn      eval_table     [AST_TYPE_COUNT];
    AstPrintFn     print_table    [AST_TYPE_COUNT];
    AstTypeFn      type_table     [AST_TYPE_COUNT];
} Runtime;

/* ================================
   Dispatch Helpers
   ================================ */



static inline bool ast_validate(ASTBase* node, EvalContext* ctx) {
    if (!node) return true;
    return ctx->runtime->validate_table[node->type](node, ctx);
}

static inline void ast_dtor(ASTBase* node, EvalContext* ctx) {
    if (!node) return;
    ctx->runtime->dtor_table[node->type](node, ctx);
}

static inline Value ast_eval(ASTBase* node, EvalContext* ctx) {
    if (!node) return (Value){.type = VAL_NULL};
    return ctx->runtime->eval_table[node->type](node, ctx);
}

static inline void ast_print(ASTBase* node, EvalContext* ctx) {
    if (!node) return;
    ctx->runtime->print_table[node->type](node, ctx);
}
static inline ValueType ast_get_type(ASTBase* node, EvalContext* ctx) {
    if (!node) return VAL_NULL;
    return ctx->runtime->type_table[node->type](node, ctx);
}

/* ================================
   Registration API
   ================================ */

void ast_register(Runtime* rt,
                  AST_TYPE type,
                  AstValidateFn validater,
                  AstDtorFn dtor,
                  AstEvalFn eval,
                  AstPrintFn print,
                  AstTypeFn get_type);

/* Initialize tables to defaults */
void runtime_init(Runtime* rt);
void ast_register_all(Runtime* rt);

#endif
