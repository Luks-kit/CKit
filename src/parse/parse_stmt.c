// parse_stmt.c
#include "lexer.h"
#include "parse_funcs.h"
#include <stdlib.h>
#include <string.h>
#include "block.h"
#include "if.h"
#include "decl.h"
#include "value.h"
#include "while.h"
#include "fndecl.h"

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

ValueType parse_type_keyword(Parser* parser) {
    if (match_token(parser, TOKEN_INT))   return VAL_INT;
    if (match_token(parser, TOKEN_STR))   return VAL_STRING;
    if (match_token(parser, TOKEN_BOOL))  return VAL_BOOL;
    if (match_token(parser, TOKEN_FLOAT)) return VAL_FLOAT;
    if (match_token(parser, TOKEN_NULL))  return VAL_NULL;
    // If we get here, the user wrote something like "fn add(not_a_type a)"
    parser_error(parser, "Expect type identifier (int, str, bool).");
    return VAL_NULL; 
}

ASTBase* parse_fn_declaration(Parser* parser) {
   
    //0. Return type

    ValueType ret_type = parse_type_keyword(parser);


    // 1. Function Name
    consume(parser, TOKEN_IDENTIFIER, "Expect function name.");
    Token name_tok = parser->previous;

    // 2. Parameters
    consume(parser, TOKEN_LPAREN, "Expect '(' after function name.");
   
    
    size_t count = 0;
    Parameter* params = malloc(sizeof(Parameter) * 32); // Simple fixed-size for now

    if (!check_token(parser, TOKEN_RPAREN)) {
        do {
            if (count >= 32) parser_error(parser, "Too many parameters.");

            // Parameter looks like: "int a" or "str name"
            ValueType p_type = parse_type_keyword(parser); // Helper to consume int/str/bool
            consume(parser, TOKEN_IDENTIFIER, "Expect parameter name.");
            
            params[count].name = strndup(parser->previous.start, parser->previous.length);
            params[count].name_len = parser->previous.length;
            params[count].type = p_type;
            count++;
        } while (match_token(parser, TOKEN_COMMA));
    }
    consume(parser, TOKEN_RPAREN, "Expect ')' after parameters.");

    // 3. Body
    consume(parser, TOKEN_LBRACE, "Expect '{' before function body.");
    ASTBase* body = parse_block(parser);

    return fndecl_new(name_tok.start, name_tok.length, params, count, ret_type, body);
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
    
    if (check_token(parser, TOKEN_FN)){
        advance(parser); // Consume the fn keyword
        return parse_fn_declaration(parser);
    }

    return parse_expr_stmt(parser);
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


