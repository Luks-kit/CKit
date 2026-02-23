// parse_stmt.c
#include "lexer.h"
#include "parse_funcs.h"
#include <stdlib.h>
#include "block.h"
#include "assign.h"
#include "ident.h"

ASTBase* parse_stmt(Parser* parser) {
    if (match_token(parser, TOKEN_LBRACE)) return parse_block(parser);
    if (check_token(parser, TOKEN_IDENTIFIER) 
        && lexer_peek_next(&parser->lexer) == '=') 
        return parse_assign(parser);
    return parse_expr_stmt(parser);
}

ASTBase* parse_assign(Parser* parser) {
    advance(parser);
    ASTBase* id = identifier_new(parser->previous.start, parser->previous.length);
    consume(parser, TOKEN_ASSIGN, "Expect '=' after identifier.");
    ASTBase* value = parse_expr(parser);
    if (!check_token(parser, TOKEN_EOF)) 
        consume(parser, TOKEN_SEMICOLON, "Expect ';' after value.");
    return assign_new(id, value);
}

ASTBase* parse_expr_stmt(Parser* parser) {
    ASTBase* expr = parse_expr(parser);  
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after expression."); 
    return expr;
}

void synchronize(Parser* parser) {
    parser->had_error = false;

    while (parser->current.type != TOKEN_EOF) {
        // If the last token was a semicolon, we are likely at the start of a new statement
        if (parser->previous.type == TOKEN_SEMICOLON) return;

        switch (parser->current.type) {
            // Add keywords here that usually start a new block/statement
            case TOKEN_LBRACE: // {
            case TOKEN_RBRACE: // }
            case TOKEN_IDENTIFIER:
                return;
            default:
                ; // Keep skipping
        }

        advance(parser);
    }
}

// Internal helper to collect statements into a block
static ASTBase* parse_statements_until(Parser* parser, TokenType sentinel) {
    ASTBase** stmts = NULL;
    size_t count = 0;
    size_t capacity = 0;

    while (!check_token(parser, sentinel) && !check_token(parser, TOKEN_EOF)) {
        ASTBase* stmt = parse_stmt(parser);
        if (stmt != NULL) {
            if (count >= capacity) {
                capacity = (capacity == 0) ? 8 : capacity * 2;
                ASTBase** next = realloc(stmts, capacity * sizeof(ASTBase*));
                if (!next) {
                    // Handle allocation failure
                    return NULL; 
                }
                stmts = next;
            }
            stmts[count++] = stmt;
        } else {
            synchronize(parser);
        }
    }

    // Pass the temp array to block_new (which copies it)
    ASTBase* block = block_new(stmts, count);

    // CRITICAL: block_new malloced its own storage and copied the stmts.
    // We must free our temporary parsing array.
    free(stmts);

    return block;
}

ASTBase* parse_block(Parser* parser) {
    // Assume '{' was already consumed by the caller
    ASTBase* block = parse_statements_until(parser, TOKEN_RBRACE);
    consume(parser, TOKEN_RBRACE, "Expect '}' after block.");
    return block;
}

ASTBase* parse_program(Parser* parser) {
    // Fill the first token
    advance(parser);

    // Parse until the end of the file
    return parse_statements_until(parser, TOKEN_EOF);
}


