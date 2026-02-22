#ifndef AST_BINOP_H
#define AST_BINOP_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value binop_eval(ASTBase* base, EvalContext* ctx);
void  binop_print(ASTBase* base, EvalContext* ctx);

#endif
