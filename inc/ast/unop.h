#ifndef AST_UNOP_H
#define AST_UNOP_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value unop_eval(ASTBase* base, EvalContext* ctx);
void  unop_print(ASTBase* base, EvalContext* ctx);
bool  unop_validate(ASTBase* base, EvalContext* ctx);
ValueType unop_get_type(ASTBase* base, EvalContext* ctx);

ASTBase* unop_new(char op, ASTBase* operand);
void unop_destroy(ASTBase* base, EvalContext* ctx);
#endif
