#ifndef WHILE_H
#define WHILE_H

#include "ast.h"
#include "eval_context.h"

Value while_eval(ASTBase* base, EvalContext* ctx);
void while_print(ASTBase* base, EvalContext* ctx);
bool while_validate(ASTBase* base, EvalContext* ctx);
ValueType while_get_type(ASTBase* base, EvalContext* ctx);

ASTBase* while_new(ASTBase* cond, ASTBase* body);
void while_destroy(ASTBase* node, EvalContext* ctx);


#endif
