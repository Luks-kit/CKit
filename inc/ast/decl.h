#ifndef DECL_H
#define DECL_H

#include "ast.h"
#include "eval_context.h"

Value decl_eval(ASTBase* base, EvalContext* ctx);
void decl_print(ASTBase* base, EvalContext* ctx);

ASTBase* decl_new(ValueType type, const char* name, size_t len, ASTBase* init);
void decl_destroy(ASTBase* base, EvalContext* ctx);


#endif
