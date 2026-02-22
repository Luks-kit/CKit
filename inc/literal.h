#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

/* Literal behavior */
Value literal_eval(ASTBase* base, EvalContext* ctx);
void  literal_print(ASTBase* base, EvalContext* ctx);

#endif
