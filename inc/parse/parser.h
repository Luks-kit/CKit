#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>
#include "ast_runtime.h"
#include "lexer.h"

typedef struct {
    Lexer lexer;
    Token current;
    Token previous;
    bool had_error;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGN,  // =
    PREC_EQUALITY,// == !=
    PREC_COMPARISON, // < >
    PREC_TERM,    // + -
    PREC_FACTOR,  // * /
    PREC_UNARY,   // -x
    PREC_PRIMARY
} Precedence;

typedef ASTBase* (*PrefixParseFn)(Parser*);
typedef ASTBase* (*InfixParseFn)(Parser*, ASTBase*);

typedef struct {
    PrefixParseFn prefix;
    InfixParseFn infix;
    Precedence precedence;
} ParseRule;

// Entry point
ASTBase* parse_program(Parser* parser);

#endif
