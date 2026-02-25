#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdbool.h>
#include "value.h"
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
    AST_IF,
    AST_WHILE,
    AST_VAR_DECL,
    AST_TYPE_DECL,
    AST_FUNC_DECL,
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
    ASTBase* lhs;
    ASTBase* rhs;
    char op[3];
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
    bool is_global;
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
    ASTBase base;
    ASTBase* callee;    // Usually an AST_IDENTIFIER
    ASTBase** args;     // Array of expressions
    size_t arg_count;
} Call;

typedef struct {
    ASTBase base;
    ValueType type;      // The "Strong" part of the typing
    const char* name;
    size_t name_len;
    ASTBase* initializer; // Optional (e.g., int x; vs int x = 5;)
} Declaration;

/* --- If Statement Node --- */
typedef struct {
    ASTBase base;
    ASTBase* condition;
    ASTBase* then_branch;
    ASTBase* else_branch; // NULL if no else
} IfStmt;

/* --- While Loop Node --- */
typedef struct {
    ASTBase base;
    ASTBase* condition;
    ASTBase* body;
} WhileStmt;


typedef struct {
    char* name;
    size_t name_len;
    ValueType type;
} Parameter;

typedef struct {
    Parameter* params;      // Array of parameter names
    size_t param_count;
    ASTBase* body;      // The Block AST node to execute
    // Optional: Env* closure; // If you want to support closures later
} UserFunc;

typedef struct Function {
    bool is_native;
    int arity;
    const char* name;
    size_t name_len;
    union {
        NativeFn native;
        UserFunc user;
    };
} Function;

typedef struct {
    ASTBase base;
    char* name;
    size_t name_len;
    Parameter* params;
    size_t param_count;
    ASTBase* body; // Usually an AST_BLOCK
} FnDecl;

#endif /* AST_H */
