#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_BOOL_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_IDENTIFIER,

    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_VAR,
    TOKEN_RETURN,
    TOKEN_TRUE,
    TOKEN_FALSE,

    TOKEN_FN, 
    TOKEN_LET, 
    TOKEN_INT,
    TOKEN_FLOAT, 
    TOKEN_BOOL, 
    TOKEN_STR, 
    TOKEN_PRINT,
    
    // Operators
    TOKEN_PLUS,        // +
    TOKEN_MINUS,       // -
    TOKEN_MUL,         // *
    TOKEN_DIV,         // /
    TOKEN_ASSIGN,      // =
    TOKEN_NEGATE,      // !
    TOKEN_LPAREN,      // (
    TOKEN_RPAREN,      // )
    TOKEN_LBRACE,      // {
    TOKEN_RBRACE,      // }
    TOKEN_COLON,       // :
    TOKEN_SEMICOLON,   // ;

    // Comparison operators
    TOKEN_LT,          // <
    TOKEN_GT,          // >
    TOKEN_LEQ,         // <=
    TOKEN_GEQ,         // >= 
    TOKEN_EQ,          // ==
    TOKEN_NEQ,         // !=
    
    TOKEN_COMMA,
    TOKEN_UNKNOWN
} TokenType;



typedef struct {
    TokenType type;
    const char* start;  // pointer into input string
    size_t length;
    int line;           // for error reporting
} Token;

static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }
static inline bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
static inline bool is_alnum(char c) { return is_alpha(c) || is_digit(c); }

typedef struct {
    const char* start;   // start of current token
    const char* current; // current scanning position
    int line;            // current line
} Lexer;

void lexer_init(Lexer* lexer, const char* source);

static inline char lexer_peek(Lexer* lexer) { return *lexer->current; }
static inline char lexer_peek_next(Lexer* lexer) { return *(lexer->current + 1); }

char lexer_advance(Lexer* lexer);

Token lexer_next_token(Lexer* lexer);

#endif
