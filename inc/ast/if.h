#ifndef IF_H
#define IF_H

#include "ast.h"
#include "eval_context.h"

Value if_eval(ASTBase* base, EvalContext* ctx);
void if_print(ASTBase* base, EvalContext* ctx);
ASTBase* if_new(ASTBase* condition, ASTBase* true_branch, ASTBase* false_branch);
void if_destroy(ASTBase* base, EvalContext* ctx);

#endif

