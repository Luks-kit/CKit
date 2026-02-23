#ifndef AST_ASSIGN_H
#define AST_ASSIGN_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value assign_eval(ASTBase* base, EvalContext* ctx);
void  assign_print(ASTBase* base, EvalContext* ctx);

ASTBase* assign_new(ASTBase* target, ASTBase* value);
void assign_destroy(ASTBase* node, EvalContext* ctx);

#endif
