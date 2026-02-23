#ifndef AST_BINOP_H
#define AST_BINOP_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value binop_eval(ASTBase* base, EvalContext* ctx);
void  binop_print(ASTBase* base, EvalContext* ctx);

ASTBase* binop_new(char op, ASTBase* lhs, ASTBase* rhs);
void binop_destroy(ASTBase* node, EvalContext* ctx);

#endif
