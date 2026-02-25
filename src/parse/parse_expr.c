// parse_expr.c
#include <stdlib.h>
#include "lexer.h"
#include "parse_funcs.h"
#include "ast.h"
#include "binop.h"
#include "parser.h"
#include "unop.h"
#include "literal.h"
#include "ident.h"
#include "assign.h"
#include "call.h"

ASTBase* parse_precedence(Parser* parser, Precedence prec) {
    advance(parser);
    PrefixParseFn prefix_rule = get_rule(parser->previous.type).prefix;
    if (!prefix_rule) {
        parser_error(parser, "Expected expression.");
        return NULL;
    }

    ASTBase* left = prefix_rule(parser);

    while (prec <= get_rule(parser->current.type).precedence) {
        advance(parser);
        InfixParseFn infix_rule = get_rule(parser->previous.type).infix;
        left = infix_rule(parser, left);
    }

    return left;
}

ASTBase* parse_expr(Parser* parser) {
    return parse_precedence(parser, PREC_ASSIGN);
}


ASTBase* parse_number(Parser* parser) {
    Token t = parser->previous;
    if (t.type == TOKEN_INT_LITERAL) 
        return literal_new_int(atoi(t.start));
    else if (t.type == TOKEN_FLOAT_LITERAL) 
        return literal_new_float(atof(t.start));
    return NULL;
}

ASTBase* parse_identifier(Parser* parser) {
    return identifier_new(parser->previous.start, parser->previous.length);
}

ASTBase* parse_string(Parser* parser) {
    Token t = parser->previous;
    return literal_new_string(t.start + 1, t.length - 2);
}

ASTBase* parse_bool(Parser* parser) {
    Token t = parser->previous;
    bool value = (t.start[0] == 't'); 
    return literal_new_bool(value);

}

ASTBase* parse_assign(Parser* parser, ASTBase* left) {
    // 1. Ensure the left-hand side is actually something we can assign to
    if (left->type != AST_IDENTIFIER) {
        parser_error(parser, "Invalid assignment target.");
        return left; 
    }

    ASTBase* value = parse_precedence(parser, PREC_ASSIGN);

    return assign_new(left, value);
}

ASTBase* parse_grouping(Parser* parser) {
    ASTBase* expr = parse_expr(parser);
    consume(parser, TOKEN_RPAREN, "Expect ')' after expression.");
    return expr;
}

ASTBase* parse_unary(Parser* parser) {
    Token op = parser->previous;
    ASTBase* right = parse_precedence(parser, PREC_UNARY);
    return unop_new(op.start[0], right);
}

ASTBase* parse_binary(Parser* parser, ASTBase* left) {
    Token operator = parser->previous;
    ParseRule rule = get_rule(operator.type);
    ASTBase* right = parse_precedence(parser, rule.precedence + 1);
    return binop_new(left, right, operator.start, operator.length);
}

// The implementation:
ASTBase* parse_call(Parser* parser, ASTBase* left) {
    ASTBase* args[64]; // Use a dynamic array for real use
    size_t count = 0;

    if (!check_token(parser, TOKEN_RPAREN)) {
        do {
            args[count++] = parse_expr(parser);
        } while (match_token(parser, TOKEN_COMMA));
    }
    consume(parser, TOKEN_RPAREN, "Expect ')' after arguments.");
    
    return call_new(left, args, count);
}


ParseRule rules[] = {
    [TOKEN_INT_LITERAL]   = {parse_number, NULL,   PREC_NONE},
    [TOKEN_FLOAT_LITERAL] = {parse_number, NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]    = {parse_identifier, NULL, PREC_NONE},
    [TOKEN_STRING_LITERAL]= {parse_string, NULL, PREC_NONE},
    [TOKEN_BOOL_LITERAL]  = {parse_bool, NULL, PREC_NONE},
    [TOKEN_LPAREN]        = {parse_grouping, parse_call, PREC_CALL},

    [TOKEN_PLUS]          = {NULL, parse_binary, PREC_TERM},
    [TOKEN_MINUS]         = {parse_unary, parse_binary, PREC_TERM},
    [TOKEN_NEGATE]        = {parse_unary, NULL, PREC_NONE},
    [TOKEN_MUL]           = {NULL, parse_binary, PREC_FACTOR},
    [TOKEN_DIV]           = {NULL, parse_binary, PREC_FACTOR},

    [TOKEN_ASSIGN]        = {NULL, parse_assign, PREC_ASSIGN},
    [TOKEN_EQ]            = {NULL, parse_binary, PREC_EQUALITY},
    [TOKEN_NEQ]           = {NULL, parse_binary, PREC_EQUALITY},
    [TOKEN_LT]            = {NULL, parse_binary, PREC_COMPARISON},
    [TOKEN_GT]            = {NULL, parse_binary, PREC_COMPARISON},
    [TOKEN_LEQ]           = {NULL, parse_binary, PREC_COMPARISON},
    [TOKEN_GEQ]           = {NULL, parse_binary, PREC_COMPARISON},
};

ParseRule get_rule(TokenType type) {
    return rules[type];
}
