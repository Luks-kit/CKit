#include "ast_runtime.h"
#include <stdio.h>

static bool default_validater(ASTBase* node, EvalContext* ctx)  { (void)node; return false; }
static void default_dtor(ASTBase* node, EvalContext* ctx)  { (void)node; }
static Value default_eval(ASTBase* node, EvalContext* ctx) {
    (void)node; (void)ctx; return (Value){ .type = VAL_NULL }; }
static void default_print(ASTBase* node, EvalContext* ctx) { (void)node; printf("<unknown>"); }
static ValueType default_get_type(ASTBase* node, EvalContext* ctx) { (void)node; return VAL_NULL; }
void runtime_init(Runtime* rt) {
    for (int i = 0; i < AST_TYPE_COUNT; i++) {
        rt->validate_table[i]  = default_validater;
        rt->dtor_table[i]  = default_dtor;
        rt->eval_table[i]  = default_eval;
        rt->print_table[i] = default_print;
        rt->type_table[i] = default_get_type;
    }
}

void ast_register(Runtime* rt,
                  AST_TYPE type,
                  AstValidateFn validater,
                  AstDtorFn dtor,
                  AstEvalFn eval,
                  AstPrintFn print,
                  AstTypeFn  get_type) {
    rt->validate_table[type]  = validater  ? validater  : default_validater;
    rt->dtor_table[type]  = dtor  ? dtor  : default_dtor;
    rt->eval_table[type]  = eval  ? eval  : default_eval;
    rt->print_table[type] = print ? print : default_print;
    rt->type_table[type] = get_type ? get_type : default_get_type;
}
