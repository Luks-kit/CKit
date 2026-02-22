#ifndef AST_BLOCK_H
#define AST_BLOCK_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value block_eval(ASTBase* base, EvalContext* ctx);
void  block_print(ASTBase* base, EvalContext* ctx);

#endif
