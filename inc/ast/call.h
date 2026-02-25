#ifndef AST_CALL_H
#define AST_CALL_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"
#include "env.h"
#include "value.h"

Value call_eval(ASTBase* base, EvalContext* ctx);
void  call_print(ASTBase* base, EvalContext* ctx);
bool  call_validate(ASTBase* base, EvalContext* ctx);
ValueType call_get_type(ASTBase* base, EvalContext* ctx);

ASTBase* call_new(ASTBase* callee, ASTBase** args, size_t count);
void call_destroy(ASTBase* node, EvalContext* ctx);
void setup_globals(Env* global_env);

#endif
