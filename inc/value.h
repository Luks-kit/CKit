#ifndef VALUE_H
#define VALUE_H

#include <stddef.h>

/* ================================
   Value Type Enum
   ================================ */

typedef enum {
    VAL_NULL,
    VAL_INT,
    VAL_FLOAT,
    VAL_BOOL,
    VAL_STRING,
    VAL_ARRAY,
    VAL_OBJECT,
    VAL_FUNCTION
} ValueType;


/* Forward declarations for recursive types */
struct Value;
struct Array;
struct Object;
struct Function;


/* ================================
   String
   ================================ */

typedef struct {
    const char* data;
    size_t length;
} String;


/* ================================
   Array
   ================================ */

typedef struct Array {
    struct Value* items;
    size_t count;
    size_t capacity;
} Array;


/* ================================
   Object (simple key-value map)
   ================================ */


struct Object;


/* ================================
   Function Value
   ================================ */

/* Native function pointer signature */
typedef struct Value (*NativeFn)(struct Value* args, size_t arg_count);

typedef struct Function {
    NativeFn native;
    void* userdata;   /* optional extension hook */
} Function;


/* ================================
   Main Value Struct
   ================================ */

typedef struct Value {
    ValueType type;

    union {
        long    i;
        double  f;
        int     b;
        String  s;
        Array*  arr;
        struct Object* object;
        Function* function;
    };

} Value;


typedef struct {
    const char* key;
    size_t key_length;
    struct Value value;
} ObjectEntry;

typedef struct Object {
    ObjectEntry* entries;
    size_t count;
    size_t capacity;
} Object;


static inline const char* type_to_string(ValueType type) {
    switch (type) {
        case VAL_INT:    return "int";
        case VAL_FLOAT:  return "float";
        case VAL_BOOL:   return "bool";
        case VAL_STRING: return "str";
        case VAL_NULL:   return "null";
        // ... add others
        default:         return "unknown";
    }
}


#endif /* VALUE_H */
