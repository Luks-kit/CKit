// parse_stmt.c
#include "parse_funcs.h"

ASTBase* parse_stmt(Parser* parser) {
    if (match_token(parser, TOKEN_LBRACE)) return parse_block(parser);
    if (check_token(parser, TOKEN_IDENTIFIER) 
        && match_token(parser, TOKEN_ASSIGN)) 
        return parse_assign(parser);
    return parse_expr_stmt(parser);
}

ASTBase* parse_block(Parser* parser) {
    // We assume `{` has already been consumed
    ASTBase** stmts = NULL;
    size_t count = 0;
    size_t capacity = 0;

    while (!check_token(parser, TOKEN_RBRACE) 
        && parser->current.type != TOKEN_EOF) {
        
        ASTBase* stmt = parse_stmt(parser);
        if (!stmt) break; // stop on parse error

        // grow array if needed
        if (count >= capacity) {
            capacity = capacity == 0 ? 4 : capacity * 2;
            stmts = realloc(stmts, capacity * sizeof(ASTBase*));
        }
        stmts[count++] = stmt;
    }

    consume(parser, TOKEN_RBRACE, "Expect '}' after block.");

    return block_new(stmts, count); // Your ASTBlock ctor
}

ASTBase* parse_assign(Parser* parser) {
    ASTBase* id = identifier_new(parser->previous.start, parser->previous.length);
    consume(parser, TOKEN_ASSIGN, "Expect '=' after identifier.");
    ASTBase* value = parse_expr(parser);
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after value.");
    return assign_new(id, value);
}

ASTBase* parse_expr_stmt(Parser* parser) {
    ASTBase* expr = parse_expr(parser);  
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after expression."); 
    return expr;
}