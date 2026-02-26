#ifndef FN_H
#define FN_H

#include "ast_runtime.h"
#include "eval_context.h"
#include "value.h"

ASTBase* fndecl_new(
        const char* name, size_t name_len, 
        Parameter* params, size_t param_count, 
        ValueType ret_type, ASTBase* body);
Value fndecl_eval(ASTBase* base, EvalContext* ctx);
void fndecl_print(ASTBase* node, EvalContext* ctx);
void fndecl_destroy(ASTBase* base, EvalContext* ctx);
bool fndecl_validate(ASTBase* node, EvalContext* ctx);
ValueType fndecl_get_type(ASTBase* node, EvalContext* ctx);


#endif
