#ifndef AST_IDENTIFIER_H
#define AST_IDENTIFIER_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value identifier_eval(ASTBase* base, EvalContext* ctx);
void  identifier_print(ASTBase* base, EvalContext* ctx);

#endif
