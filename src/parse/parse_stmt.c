// parse_stmt.c
#include "lexer.h"
#include "parse_funcs.h"
#include <stdlib.h>
#include "block.h"
#include "assign.h"
#include "ident.h"
#include "if.h"
#include "decl.h"
#include "while.h"

ASTBase* parse_declaration(Parser* parser) {
    // The type token was already advanced in parse_stmt, so it's in 'previous'
    ValueType target_type = map_token_to_type(parser->previous.type);

    consume(parser, TOKEN_IDENTIFIER, "Expect variable name after type.");
    Token name = parser->previous;

    ASTBase* initializer = NULL;
    if (match_token(parser, TOKEN_ASSIGN)) {
        initializer = parse_expr(parser);
    }

    consume(parser, TOKEN_SEMICOLON, "Expect ';' after variable declaration.");
    return decl_new(target_type, name.start, name.length, initializer);
}

ASTBase* parse_if(Parser* parser) {
    consume(parser, TOKEN_LPAREN, "Expect '(' after 'if'.");
    ASTBase* condition = parse_expr(parser);
    consume(parser, TOKEN_RPAREN, "Expect ')' after if condition.");

    ASTBase* then_branch = parse_stmt(parser);
    ASTBase* else_branch = NULL;

    if (match_token(parser, TOKEN_ELSE)) {
        else_branch = parse_stmt(parser);
    }

    return if_new(condition, then_branch, else_branch);
}

ASTBase* parse_while(Parser* parser) {
    consume(parser, TOKEN_LPAREN, "Expect '(' after 'while'.");
    ASTBase* condition = parse_expr(parser);
    consume(parser, TOKEN_RPAREN, "Expect ')' after while condition.");

    ASTBase* body = parse_stmt(parser);

    return while_new(condition, body);
}


ASTBase* parse_stmt(Parser* parser) {
    if (match_token(parser, TOKEN_LBRACE)) {
        return parse_block(parser);
    }
    if (match_token(parser, TOKEN_IF)) {
        return parse_if(parser);
    }
    if (match_token(parser, TOKEN_WHILE)) {
        return parse_while(parser);
    }
    
    // Check if the current token is a Type keyword (int, str, bool, float)
    if (check_token(parser, TOKEN_INT) || check_token(parser, TOKEN_FLOAT) ||
        check_token(parser, TOKEN_STR) || check_token(parser, TOKEN_BOOL)) {
        advance(parser); // Consume the type token
        return parse_declaration(parser);
    }

    // Lookahead for assignment: IDENTIFIER followed by '='
    if (check_token(parser, TOKEN_IDENTIFIER) && lexer_peek(&parser->lexer) == '=') {
        return parse_assign(parser);
    }

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
        if (parser->previous.type == TOKEN_SEMICOLON) return;

        switch (parser->current.type) {
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_INT:
            case TOKEN_STR:
            case TOKEN_BOOL:
            case TOKEN_FLOAT:
            case TOKEN_LBRACE:
                return;
            default:
                break;
        }

        advance(parser);
    }
}// Internal helper to collect statements into a block
static ASTBase* parse_statements_until(Parser* parser, TokenType sentinel, bool isGlobal) {
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
    ASTBase* block = block_new(stmts, count, isGlobal);

    // CRITICAL: block_new malloced its own storage and copied the stmts.
    // We must free our temporary parsing array.
    free(stmts);

    return block;
}

ASTBase* parse_block(Parser* parser) {
    // Assume '{' was already consumed by the caller
    ASTBase* block = parse_statements_until(parser, TOKEN_RBRACE, false);
    consume(parser, TOKEN_RBRACE, "Expect '}' after block.");
    return block;
}

ASTBase* parse_program(Parser* parser) {
    // Fill the first token
    advance(parser);

    // Parse until the end of the file
    return parse_statements_until(parser, TOKEN_EOF, true);
}


