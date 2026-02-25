#include "literal.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Value literal_eval(ASTBase* base, EvalContext* ctx)
{
    (void)ctx;
    Literal* lit = (Literal*)base;

    switch (lit->lit_type) {
        case AST_LIT_INT:
            return (Value){ .type = VAL_INT,
                            .i = ((IntLiteral*)base)->value };

        case AST_LIT_FLOAT:
            return (Value){ .type = VAL_FLOAT,
                            .f = ((FloatLiteral*)base)->value };

        case AST_LIT_BOOL:
            return (Value){ .type = VAL_BOOL,
                            .b = ((BoolLiteral*)base)->value };

        case AST_LIT_STR: {
            StrLiteral* s = (StrLiteral*)base;
            return (Value){ .type = VAL_STRING,
                            .s = { strndup(s->data, s->length), s->length } };
        }
    }

    return (Value){ .type = VAL_NULL };
}

ValueType literal_get_type(ASTBase* node, EvalContext* ctx) {
    if (!node) return VAL_NULL; 
    Literal* lit = (Literal*)node;
     switch (lit->lit_type) {
         case(AST_LIT_INT): return VAL_INT;
         case(AST_LIT_BOOL): return VAL_BOOL;
         case(AST_LIT_FLOAT): return VAL_FLOAT;
         case(AST_LIT_STR): return VAL_STRING; 
    }
}

bool literal_validate(ASTBase* node, EvalContext* ctx) {
    return true; // Ints, Bools, and Strings are structurally sound
}

void literal_print(ASTBase* base, EvalContext* ctx)
{
    Literal* lit = (Literal*)base;

    switch (lit->lit_type) {
        case AST_LIT_INT:
            printf("%ld", ((IntLiteral*)base)->value);
            break;

        case AST_LIT_FLOAT:
            printf("%f", ((FloatLiteral*)base)->value);
            break;

        case AST_LIT_BOOL:
            printf("%s",
                   ((BoolLiteral*)base)->value ? "true" : "false");
            break;

        case AST_LIT_STR: {
            StrLiteral* s = (StrLiteral*)base;
            printf("\"%.*s\"",
                   (int)s->length,
                   s->data);
            break;
        }
    }
}



ASTBase* literal_new_int(long value) {
    IntLiteral* lit = malloc(sizeof(IntLiteral));
    lit->literal.base.type = AST_LITERAL;
    lit->literal.lit_type = AST_LIT_INT;
    lit->value = value;
    return &lit->literal.base;
}

ASTBase* literal_new_string(const char* data, size_t length) {
    StrLiteral* lit = malloc(sizeof(StrLiteral));
    lit->literal.base.type = AST_LITERAL;
    lit->literal.lit_type = AST_LIT_STR;

    char* copy = malloc(length);
    memcpy(copy, data, length);

    lit->data = copy;
    lit->length = length;

    return &lit->literal.base;
}

ASTBase* literal_new_float(double value) {
    FloatLiteral* lit = malloc(sizeof(FloatLiteral));
    lit->literal.base.type = AST_LITERAL;
    lit->literal.lit_type = AST_LIT_FLOAT;
    lit->value = value;
    
    return &lit->literal.base;
}


ASTBase* literal_new_bool(int value) {
    BoolLiteral* lit = malloc(sizeof(BoolLiteral));
    lit->literal.base.type = AST_LITERAL;
    lit->literal.lit_type = AST_LIT_BOOL;
    lit->value = value;

    return &lit->literal.base;
}

void literal_destroy(ASTBase* node, EvalContext* ctx) {
    Literal* lit = (Literal*)node;

    if (lit->lit_type == AST_LIT_STR) {
        StrLiteral* s = (StrLiteral*)node;
        free((void*)s->data);
    }

    free(node);
}
