#include "literal.h"
#include <stdio.h>

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
                            .s = { s->data, s->length } };
        }
    }

    return (Value){ .type = VAL_NULL };
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
