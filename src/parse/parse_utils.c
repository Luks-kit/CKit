#include "parse_funcs.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Moves the parser to the next token in the stream.
 * It updates 'previous' to 'current' and 'current' to the new token from the lexer.
 */
Token advance(Parser* parser) {
    parser->previous = parser->current;

    for (;;) {
        parser->current = lexer_next_token(&parser->lexer);

        // Skip error tokens from the lexer and report them
        if (parser->current.type != TOKEN_UNKNOWN) break;

        parser_error(parser, parser->current.start);
    }

    return parser->current;
}

/**
 * Checks if the current token matches a specific type.
 */
bool check_token(Parser* parser, TokenType type) {
    return parser->current.type == type;
}

/**
 * If the current token matches 'type', it advances and returns true.
 * Otherwise, it returns false without advancing.
 */
bool match_token(Parser* parser, TokenType type) {
    if (!check_token(parser, type)) return false;
    advance(parser);
    return true;
}

/**
 * Requires the current token to be of 'type'.
 * If it is, it advances. If not, it reports a syntax error.
 */
void consume(Parser* parser, TokenType type, const char* message) {
    if (parser->current.type == type) {
        advance(parser);
        return;
    }

    parser_error(parser, message);
}

/**
 * Basic error reporting. Sets the error flag to prevent
 * cascading issues (like trying to execute a broken AST).
 */
void parser_error(Parser* parser, const char* message) {
    if (parser->had_error) return; // Prevent error spam
    parser->had_error = true;

    fprintf(stderr, "[line %d] Error at '%.lu': %s\n",
            parser->current.line,
            parser->current.length,
            message);
}

