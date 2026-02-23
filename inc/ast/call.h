#ifndef AST_CALL_H
#define AST_CALL_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value call_eval(ASTBase* base, EvalContext* ctx);
void  call_print(ASTBase* base, EvalContext* ctx);

#endif
