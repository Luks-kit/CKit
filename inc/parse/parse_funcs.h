#ifndef PARSE_FUNCS_H
#define PARSE_FUNCS_H

#include "parser.h"

// Statements
ASTBase* parse_stmt(Parser* parser);
ASTBase* parse_block(Parser* parser);
ASTBase* parse_assign(Parser* parser);
ASTBase* parse_expr_stmt(Parser* parser);

// Expressions
ASTBase* parse_expr(Parser* parser);
ASTBase* parse_binary(Parser* parser, ASTBase* left);
ASTBase* parse_grouping(Parser* parser);
ASTBase* parse_unary(Parser* parser);

// Utilities
bool match_token(Parser* parser, TokenType type);
bool check_token(Parser* parser, TokenType type);
Token advance(Parser* parser);
void consume(Parser* parser, TokenType type, const char* message);
void parser_error(Parser* parser, const char* message);

ParseRule get_rule(TokenType type);

static inline ValueType map_token_to_type(TokenType type) {
    switch (type) {
        case TOKEN_INT:   return VAL_INT;
        case TOKEN_FLOAT: return VAL_FLOAT;
        case TOKEN_STR:   return VAL_STRING;
        case TOKEN_BOOL:  return VAL_BOOL;
        default:          return VAL_NULL;
    }
}

#endif
