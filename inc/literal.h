#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

/* Literal behavior */
Value literal_eval(ASTBase* base, EvalContext* ctx);
void  literal_print(ASTBase* base, EvalContext* ctx);

ASTBase* literal_new_int(long value);
ASTBase* literal_new_float(double value);
ASTBase* literal_new_bool(int value);
ASTBase* literal_new_string(const char* data, size_t length);

void literal_destroy(ASTBase* node, EvalContext* ctx);

#endif
