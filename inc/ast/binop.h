#ifndef AST_BINOP_H
#define AST_BINOP_H

#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"

Value binop_eval(ASTBase* base, EvalContext* ctx);
void  binop_print(ASTBase* base, EvalContext* ctx);
bool  binop_validate(ASTBase* base, EvalContext* ctx);
ValueType binop_get_type(ASTBase* base, EvalContext* ctx);
ASTBase* binop_new(ASTBase* lhs, ASTBase* rhs, const char* op_str, size_t len);
void binop_destroy(ASTBase* node, EvalContext* ctx);

#endif
