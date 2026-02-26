#include "lexer.h"
#include <string.h> 

void lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

char lexer_advance(Lexer* lexer) {
    char c = *lexer->current;
    lexer->current++;
    if (c == '\n') lexer->line++;
    return c;
}

void lexer_skip_whitespace(Lexer* lexer) {
    for (;;) {
        char c = lexer_peek(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                lexer_advance(lexer);
                break;
            case '/':
                if (lexer_peek_next(lexer) == '/') {
                    while (lexer_peek(lexer) != '\n' && lexer_peek(lexer) != '\0')
                       lexer_advance(lexer);
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}


Token make_token(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->start;
    token.length = (size_t)(lexer->current - lexer->start);
    token.line = lexer->line;
    return token;
}

Token error_token(Lexer* lexer, const char* message) {
    Token token;
    token.type = TOKEN_UNKNOWN;
    token.start = message;
    token.length = strlen(message);
    token.line = lexer->line;
    return token;
}

Token lex_number(Lexer* lexer) {
    while (is_digit(lexer_peek(lexer))) lexer_advance(lexer);

    if (lexer_peek(lexer) == '.' && is_digit(lexer_peek_next(lexer))) {
        lexer_advance(lexer); // consume '.'
        while (is_digit(lexer_peek(lexer))) lexer_advance(lexer);
        return make_token(lexer, TOKEN_FLOAT_LITERAL);
    }

    return make_token(lexer, TOKEN_INT_LITERAL);
}

Token lex_identifier(Lexer* lexer) {
    while (is_alnum(lexer_peek(lexer))) lexer_advance(lexer);
    size_t length = (size_t)(lexer->current - lexer->start);

    // Simple keyword mapping
    #define KEYWORD(s, t) \
    if (length == strlen(s) && strncmp(lexer->start, s, length) == 0) return make_token(lexer, t)
    
    KEYWORD("if",    TOKEN_IF);
    KEYWORD("else",  TOKEN_ELSE);
    KEYWORD("while", TOKEN_WHILE);
    KEYWORD("let",   TOKEN_LET);
    KEYWORD("int",   TOKEN_INT);
    KEYWORD("float", TOKEN_FLOAT);
    KEYWORD("str",   TOKEN_STR);
    KEYWORD("bool",  TOKEN_BOOL);
    KEYWORD("true",  TOKEN_BOOL_LITERAL);
    KEYWORD("false", TOKEN_BOOL_LITERAL);
    KEYWORD("fn",    TOKEN_FN);
    KEYWORD("null",  TOKEN_NULL);
    #undef KEYWORD
    return make_token(lexer, TOKEN_IDENTIFIER);
}

Token lex_string(Lexer* lexer) {
    lexer_advance(lexer); // consume opening "
    while (lexer_peek(lexer) != '"' && lexer_peek(lexer) != '\0') {
        if (lexer_peek(lexer) == '\n') lexer->line++;
        lexer_advance(lexer);
    }

    if (lexer_peek(lexer) == '\0') return error_token(lexer, "Unterminated string.");

    lexer_advance(lexer); // consume closing "
    return make_token(lexer, TOKEN_STRING_LITERAL);
}


Token lexer_next_token(Lexer* lexer) {
    lexer_skip_whitespace(lexer);
    lexer->start = lexer->current;

    if (*lexer->current == '\0') return make_token(lexer, TOKEN_EOF);

    char c = lexer_advance(lexer);

    if (is_digit(c)) return lex_number(lexer);
    if (is_alpha(c)) return lex_identifier(lexer);

    switch (c) {
	case '"': return lex_string(lexer);
        case '+': return make_token(lexer, TOKEN_PLUS);
        case '-': return make_token(lexer, TOKEN_MINUS);
        case '*': return make_token(lexer, TOKEN_MUL);
        case '/': return make_token(lexer, TOKEN_DIV);
        case '=':
            if (lexer_peek(lexer) == '=') 
            { lexer_advance(lexer); return make_token(lexer, TOKEN_EQ); }
            else return make_token(lexer, TOKEN_ASSIGN);
        case '!':
            if (lexer_peek(lexer) == '=')
            { lexer_advance(lexer); return make_token(lexer, TOKEN_NEQ); }
            else return make_token(lexer, TOKEN_NEGATE);
        case '<': 
            if (lexer_peek(lexer) == '=') 
            { lexer_advance(lexer); return make_token(lexer, TOKEN_LEQ); }
            else return make_token(lexer, TOKEN_LT);
        case '>': 
            if (lexer_peek(lexer) == '=')
            { lexer_advance(lexer); return make_token(lexer, TOKEN_GEQ); }
            else return make_token(lexer, TOKEN_GT);
        case '(': return make_token(lexer, TOKEN_LPAREN);
        case ')': return make_token(lexer, TOKEN_RPAREN);
        case '{': return make_token(lexer, TOKEN_LBRACE);
        case '}': return make_token(lexer, TOKEN_RBRACE);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case ';': return make_token(lexer, TOKEN_SEMICOLON);
    }

    return error_token(lexer, "Unexpected character.");
}
