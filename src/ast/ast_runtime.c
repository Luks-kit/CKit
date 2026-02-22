#include "ast_runtime.h"
#include <stdlib.h>
#include <stdio.h>

static void default_ctor(ASTBase* node)  { (void)node; }
static void default_dtor(ASTBase* node)  { (void)node; }
static Value default_eval(ASTBase* node, struct EvalContext* ctx) {
    (void)node; (void)ctx; return (Value){ .type = VAL_NULL }; }
static void default_print(ASTBase* node) { (void)node; printf("<unknown>"); }

void runtime_init(Runtime* rt) {
    for (int i = 0; i < AST_TYPE_COUNT; i++) {
        rt->ctor_table[i]  = default_ctor;
        rt->dtor_table[i]  = default_dtor;
        rt->eval_table[i]  = default_eval;
        rt->print_table[i] = default_print;
    }
}

void ast_register(Runtime* rt,
                  AST_TYPE type,
                  AstCtorFn ctor,
                  AstDtorFn dtor,
                  AstEvalFn eval,
                  AstPrintFn print) {
    rt->ctor_table[type]  = ctor  ? ctor  : default_ctor;
    rt->dtor_table[type]  = dtor  ? dtor  : default_dtor;
    rt->eval_table[type]  = eval  ? eval  : default_eval;
    rt->print_table[type] = print ? print : default_print;
}
