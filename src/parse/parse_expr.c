// parse_expr.c
#include "parse_funcs.h"


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

ASTBase* parse_grouping(Parser* parser) {
    ASTBase* expr = parse_expr(parser);
    consume(parser, TOKEN_RPAREN, "Expect ')' after expression.");
    return expr;
}

ASTBase* parse_unary(Parser* parser) {
    TokenType operator_type = parser->previous.type;
    ASTBase* right = parse_precedence(parser, PREC_UNARY);
    return unary_new(operator_type, right);
}

ASTBase* parse_binary(Parser* parser, ASTBase* left) {
    TokenType operator_type = parser->previous.type;
    ParseRule rule = get_rule(operator_type);
    ASTBase* right = parse_precedence(parser, rule.precedence + 1);
    return binop_new(operator_type, left, right);
}

ParseRule rules[] = {
    [TOKEN_INT_LITERAL]   = {parse_number, NULL,   PREC_NONE},
    [TOKEN_FLOAT_LITERAL] = {parse_number, NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]    = {parse_identifier, NULL, PREC_NONE},
    [TOKEN_LPAREN]        = {parse_grouping, NULL, PREC_NONE},

    [TOKEN_PLUS]          = {NULL, parse_binary, PREC_TERM},
    [TOKEN_MINUS]         = {parse_unary, parse_binary, PREC_TERM},
    [TOKEN_MUL]           = {NULL, parse_binary, PREC_FACTOR},
    [TOKEN_DIV]           = {NULL, parse_binary, PREC_FACTOR},

    [TOKEN_EQ]            = {NULL, parse_binary, PREC_EQUALITY},
    [TOKEN_NEQ]           = {NULL, parse_binary, PREC_EQUALITY},
    [TOKEN_LT]            = {NULL, parse_binary, PREC_COMPARISON},
    [TOKEN_GT]            = {NULL, parse_binary, PREC_COMPARISON},
};