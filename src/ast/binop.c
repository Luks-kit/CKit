#include "binop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static double to_double(Value v)
{
    if (v.type == VAL_FLOAT) return v.f;
    if (v.type == VAL_INT)   return (double)v.i;
    return 0.0;
}


Value binop_eval(ASTBase* base, EvalContext* ctx) {
    BinOp* bin = (BinOp*)base;
    Value left  = ast_eval(bin->lhs, ctx);
    Value right = ast_eval(bin->rhs, ctx);

    if (ctx->has_error) return (Value){ .type = VAL_NULL };

    // 1. Equality Operators (Works on same types)
    if (strcmp(bin->op, "==") == 0 || strcmp(bin->op, "!=") == 0) {
        bool equal = false;
        if (left.type == VAL_INT)         equal = left.i == right.i;
        else if (left.type == VAL_FLOAT)  equal = left.f == right.f;
        else if (left.type == VAL_BOOL)   equal = left.b == right.b;
        else if (left.type == VAL_STRING) equal = (left.s.length == right.s.length && 
                                                   memcmp(left.s.data, right.s.data, left.s.length) == 0);
        
        bool result = (strcmp(bin->op, "==") == 0) ? equal : !equal;
        return (Value){ .type = VAL_BOOL, .b = result };
    }

    // 2. String Concatenation
    if (left.type == VAL_STRING && right.type == VAL_STRING && strcmp(bin->op, "+") == 0) {
        Value res = { .type = VAL_STRING, .s.length = left.s.length + right.s.length };
        char* combined = malloc(res.s.length + 1);
        memcpy(combined, left.s.data, left.s.length);
        memcpy(combined + left.s.length, right.s.data, right.s.length);
        combined[res.s.length] = '\0';
        res.s.data = combined;
        return res;
    }

    // 3. Numeric Operations (Comparison & Arithmetic)
    if ((left.type == VAL_INT || left.type == VAL_FLOAT) && 
        (right.type == VAL_INT || right.type == VAL_FLOAT)) {
        
        double l = (left.type == VAL_INT) ? (double)left.i : left.f;
        double r = (right.type == VAL_INT) ? (double)right.i : right.f;

        // Comparisons
        if (strcmp(bin->op, "<") == 0)  return (Value){ .type = VAL_BOOL, .b = l < r };
        if (strcmp(bin->op, ">") == 0)  return (Value){ .type = VAL_BOOL, .b = l > r };
        if (strcmp(bin->op, "<=") == 0) return (Value){ .type = VAL_BOOL, .b = l <= r };
        if (strcmp(bin->op, ">=") == 0) return (Value){ .type = VAL_BOOL, .b = l >= r };

        // Arithmetic
        bool both_int = (left.type == VAL_INT && right.type == VAL_INT);
        if (strcmp(bin->op, "+") == 0) 
            return both_int ? (Value){VAL_INT, .i = left.i + right.i} : (Value){VAL_FLOAT, .f = l + r};
        if (strcmp(bin->op, "-") == 0) 
            return both_int ? (Value){VAL_INT, .i = left.i - right.i} : (Value){VAL_FLOAT, .f = l - r};
        if (strcmp(bin->op, "*") == 0) 
            return both_int ? (Value){VAL_INT, .i = left.i * right.i} : (Value){VAL_FLOAT, .f = l * r};
        if (strcmp(bin->op, "/") == 0) {
            if (r == 0) {
                ctx->has_error = 1;
                ctx->error_message = "Division by zero";
                return (Value){.type = VAL_NULL};
            }
            return both_int ? (Value){VAL_INT, .i = left.i / right.i} : (Value){VAL_FLOAT, .f = l / r};
        }
    }

    ctx->has_error = 1;
    ctx->error_message = "Type error: Invalid binary operation types";
    return (Value){ .type = VAL_NULL };
}

bool is_comparison_op ( const char* op){
    return 
        strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 || 
        strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 || 
        strcmp(op, "<")  == 0 || strcmp(op, ">") == 0   ;
} 

ValueType binop_get_type(ASTBase* node, EvalContext* ctx) {
    BinOp* bin = (BinOp*)node;
    // Comparisons always return bool
    if(is_comparison_op(bin->op)) return VAL_BOOL;
    
    // Math usually returns the type of the left side (or the "wider" type)
    return ast_get_type(bin->lhs, ctx);
}

bool binop_validate(ASTBase* base, EvalContext* ctx) {
    BinOp* bin = (BinOp*)base;
    if (!ast_validate(bin->lhs, ctx) || !ast_validate(bin->rhs, ctx)) return false;

    ValueType left_t = ast_get_type(bin->lhs, ctx);
    ValueType right_t = ast_get_type(bin->rhs, ctx);

    if (left_t != right_t) {
        fprintf(stderr, "Type Error: Operator '%s' cannot be applied to %d and %d.\n", 
                bin->op, left_t, right_t);
        ctx->has_error = true;
        return false;
    }
    return true;
}


void binop_print(ASTBase* base, EvalContext* ctx)
{
    BinOp* bin = (BinOp*)base;
    printf("(");
    ast_print(bin->lhs, ctx);
    printf(" %s ", bin->op);
    ast_print(bin->rhs, ctx);
    printf(")");
}


ASTBase* binop_new(ASTBase* lhs, ASTBase* rhs, const char* op_str, size_t len) {
    BinOp* bin = malloc(sizeof(BinOp));
    bin->base.type = AST_BINOP;
    bin->lhs = lhs;
    bin->rhs = rhs;
    
    // Safety check for length, then copy
    size_t copy_len = len > 2 ? 2 : len;
    memcpy(bin->op, op_str, copy_len);
    bin->op[copy_len] = '\0';
    
    return (ASTBase*)bin;
}

void binop_destroy(ASTBase* node, EvalContext* ctx) {
    BinOp* bin = (BinOp*)node;
    ast_dtor(bin->lhs, ctx);
    ast_dtor(bin->rhs, ctx);
    free(bin);
}
