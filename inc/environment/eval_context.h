#ifndef EVAL_CONTEXT_H
#define EVAL_CONTEXT_H

#include <stddef.h>
#include "value.h"

/* ================================
   Forward Declarations
   ================================ */

struct EvalContext;
struct Runtime; 
typedef struct Runtime Runtime;

/* ================================
   Environment Entry
   ================================ */

typedef struct {
    const char* name;
    size_t name_length;
    Value value;
} EnvEntry;


/* ================================
   Environment (Lexical Scope)
   ================================ */

typedef struct Env {
    EnvEntry* entries;
    size_t count;
    size_t capacity;

    struct Env* parent;
} Env;


/* ================================
   Builtin Function Entry
   ================================ */

typedef struct {
    const char* name;
    size_t name_length;
    NativeFn function;
} BuiltinEntry;


/* ================================
   Runtime Configuration
   ================================ */

typedef struct {
    int enable_trace;
    int strict_mode;
} RuntimeConfig;


/* ================================
   Eval Context
   ================================ */

typedef struct EvalContext {

    /* Global scope */
    Env* global_env;

    /* Current lexical scope */
    Env* current_env;

    /* Builtins */
    BuiltinEntry* builtins;
    size_t builtin_count;
    size_t builtin_capacity;

    /* Error handling */
    int has_error;
    const char* error_message;

    /* Optional user hook */
    void* userdata;
    Runtime* runtime;
    /* Config flags */
    RuntimeConfig config;

} EvalContext;

#endif /* EVAL_CONTEXT_H */
