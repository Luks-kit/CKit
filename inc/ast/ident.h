#ifndef AST_IDENTIFIER_H
#define AST_IDENTIFIER_H

#include "ast.h"
#include "eval_context.h"

Value identifier_eval(ASTBase* base, EvalContext* ctx);
void  identifier_print(ASTBase* base, EvalContext* ctx);
bool  identifier_validate(ASTBase* base, EvalContext* ctx);
ValueType identifier_get_type(ASTBase* base, EvalContext* ctx);

ASTBase* identifier_new(const char* name, size_t length);
void identifier_destroy(ASTBase* node, EvalContext* ctx);

#endif
