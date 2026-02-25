#include "call.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Value call_eval(ASTBase* base, EvalContext* ctx) {
    Call* call = (Call*)base;
    Value callee = ast_eval(call->callee, ctx);
    
    if (callee.type != VAL_FUNCTION) {
        ctx->has_error = true;
        ctx->error_message = strdup("Couldn't find function"); 
        return (Value){ .type = VAL_NULL };
    }

    Function* f = callee.function; 
    
    // 1. Eval args in the CALLER scope
    Value* evaluated_args = malloc(sizeof(Value) * call->arg_count);
    for (size_t i = 0; i < call->arg_count; i++) {
        evaluated_args[i] = ast_eval(call->args[i], ctx);
    }

    Value result = { .type = VAL_NULL };

    if (f->is_native) {
        result = f->native(evaluated_args, call->arg_count);
    } else {
        // 2. Setup the CALLEE scope
        // Pointing to global_env for basic lexical scoping
        Env* func_env = env_push(ctx->global_env); 
        Env* prev_env = ctx->current_env;
        ctx->current_env = func_env;

        // 3. Bind parameters
        // Inside call_eval:
    for (size_t i = 0; i < f->user.param_count; i++) {
        // Check if these are valid!
        const char* p_name = f->user.params[i].name;
        size_t p_len = f->user.params[i].name_len;
        
        env_set(ctx->current_env, p_name, p_len, evaluated_args[i]);
    }

        // 4. Run body
        result = ast_eval(f->user.body, ctx);

        // 5. Restore scope and cleanup
        ctx->current_env = prev_env;
        env_pop(func_env);
    }

    free(evaluated_args);
    return result;
}

ValueType call_get_type(ASTBase* base, EvalContext* ctx) {
    return VAL_NULL; 
}

bool call_validate(ASTBase* node, EvalContext* ctx) {
    Call* call = (Call*)node;
    if (!ast_validate(call->callee, ctx)) return false;

    // We must check if the function exists to see its parameter types
    Value func_val = ast_eval(call->callee, ctx);
    if (func_val.type != VAL_FUNCTION) {
        ctx->has_error = true;
        ctx->error_message = strdup("Value is not callable.");
        return false;
    }

    Function* f = func_val.function;

    // 1. Check Arity
    if (f->arity != -1 && f->arity != (int)call->arg_count) {
        fprintf(stderr, "Type Error: Function '%s' expects %d args, got %zu.\n", 
                f->name, f->arity, call->arg_count);
        return false;
    }

    // 2. Check Argument Types (Only for User Functions)
    if (!f->is_native) {
        for (size_t i = 0; i < call->arg_count; i++) {
            ValueType expected = f->user.params[i].type;
            ValueType actual = ast_get_type(call->args[i], ctx);

            if (expected != actual) {
                fprintf(stderr, "Type Error: Arg %zu of '%s' expects %s, got %s.\n",
                        i, f->name, type_to_string(expected), type_to_string(actual));
                return false;
            }
        }
    }

    return true;
}


void call_print(ASTBase* base, EvalContext* ctx)
{
    Call* call = (Call*)base;
    ast_print(call->callee, ctx);
    printf("(");
    for (size_t i = 0; i < call->arg_count; i++) {
        ast_print(call->args[i], ctx);
        if (i + 1 < call->arg_count) printf(", ");
    }
    printf(")");
}

Value native_print(Value* args, size_t arg_count) {
    for (size_t i = 0; i < arg_count; i++) {
        switch (args[i].type) {
            case (VAL_INT): {printf("%ld\n", args[i].i); break;}
            case (VAL_FLOAT): {printf("%f\n", args[i].f); break;}
            case (VAL_BOOL): {printf("%s\n", args[i].b ? "[true]" : "[false]"); break;}
            case (VAL_STRING): {printf("%.*s\n", (int)args[i].s.length, args[i].s.data); break;}
            case (VAL_NULL): {printf("Null Type(Nothing)\n"); break;}
            default: {printf("Unimplemented type\n"); break; }
        }

    }
    printf("\n");
    return (Value){.type = VAL_NULL};
}

// Global Setup
void setup_globals(Env* global_env) {
    Function* f = malloc(sizeof(Function));
    f->is_native = true;
    f->native = native_print;
    f->arity = 1; // Or make it variadic later
    f->name = "print";
    f->name_len = 5;
    Value v = {.type = VAL_FUNCTION, .function = f};
    env_set(global_env, "print", 5, v);
}

ASTBase* call_new(ASTBase* callee, ASTBase** args, size_t count) {
    Call* node = malloc(sizeof(Call));
    node->base.type = AST_CALL;
    node->callee = callee;
    node->arg_count = count;

    // Allocate space for the argument pointers
    if (count > 0) {
        node->args = malloc(sizeof(ASTBase*) * count);
        for (size_t i = 0; i < count; i++) {
            node->args[i] = args[i];
        }
    } else {
        node->args = NULL;
    }

    return (ASTBase*)node;
}

void call_destroy(ASTBase* node, EvalContext* ctx) {
    Call* call = (Call*)node;
    
    // 1. Destroy the callee (the name or expression)
    ast_dtor(call->callee, ctx);

    // 2. Destroy each argument expression
    for (size_t i = 0; i < call->arg_count; i++) {
        ast_dtor(call->args[i], ctx);
    }

    // 3. Free the pointer array
    free(call->args);

    // 4. Free the node itself
    free(call);
}
