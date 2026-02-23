#ifndef AST_H
#define AST_H

#include <stddef.h>

/* ================================
   AST Type Enums
   ================================ */

typedef enum {
    AST_LITERAL,
    AST_BINOP,
    AST_UNOP,
    AST_INDEX,
    AST_MEMBER,
    AST_CALL,
    AST_IDENTIFIER,
    AST_ASSIGN,
    AST_BLOCK,
    AST_TYPE_COUNT
} AST_TYPE;

typedef enum {
    AST_LIT_STR,
    AST_LIT_INT,
    AST_LIT_FLOAT,
    AST_LIT_BOOL
} AST_LIT_TYPE;


/* ================================
   Base Node
   ================================ */

typedef struct {
    AST_TYPE type;
} ASTBase;


/* ================================
   Literal Base
   ================================ */

typedef struct {
    ASTBase base;          /* MUST be first */
    AST_LIT_TYPE lit_type;
} Literal;


/* ================================
   Concrete Literal Types
   ================================ */

/* String Literal */
typedef struct {
    Literal literal;       /* MUST be first */
    const char* data;
    size_t length;         /* Explicit length for safety */
} StrLiteral;

/* Integer Literal */
typedef struct {
    Literal literal;
    long value;
} IntLiteral;

/* Float Literal */
typedef struct {
    Literal literal;
    double value;
} FloatLiteral;

/* Boolean Literal */
typedef struct {
    Literal literal;
    int value;             /* 0 or 1 */
} BoolLiteral;


/* ================================
   Identifier
   ================================ */

typedef struct {
    ASTBase base;
    const char* name;
    size_t name_length;
} Identifier;

/* ================================
   Binary Operation
   ================================ */

typedef struct {
    ASTBase base;          /* MUST be first */
    char op;
    ASTBase* lhs;
    ASTBase* rhs;
} BinOp;


/* ================================
   Unary Operation
   ================================ */

typedef struct {
    ASTBase base;          /* MUST be first */
    char op;
    ASTBase* operand;
} UnOp;


/* ================================
   Index Operation
   ================================ */

typedef struct {
    ASTBase base;          /* MUST be first */
    ASTBase* target;
    ASTBase* index;
} Index;


typedef struct {
    ASTBase base;
    ASTBase** statements;
    size_t count;
} Block;


typedef struct {
    ASTBase base;
    Identifier* target;   /* variable being assigned */
    ASTBase* value;       /* expression to evaluate */
} Assign;

/* ================================
   Member Access
   ================================ */

typedef struct {
    ASTBase base;          /* MUST be first */
    ASTBase* target;
    const char* member_name;
} Member;


/* ================================
   Function Call
   ================================ */

typedef struct {
    ASTBase base;          /* MUST be first */
    const char* name;
    ASTBase** args;
    size_t arg_count;
} Call;

#endif /* AST_H */
