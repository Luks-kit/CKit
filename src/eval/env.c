#include <string.h>
#include <stdlib.h>
#include "eval_context.h"

/* ================================
   Lookup a variable in the environment chain
   ================================ */
Value env_lookup(Env* env, const char* name, size_t len) {
    while (env) {
        for (size_t i = 0; i < env->count; i++) {
            EnvEntry* e = &env->entries[i];
            if (e->name_length == len &&
                strncmp(e->name, name, len) == 0) {
                return e->value;
            }
        }
        env = env->parent;
    }

    /* Not found */
    Value v = { .type = VAL_NULL };
    return v;
}

/* ================================
   Set variable in current env
   ================================ */
void env_set(Env* env, const char* name, size_t len, Value val) {
    
    for (size_t i = 0; i < env->count; i++) {
        EnvEntry* e = &env->entries[i];
        if (e->name_length == len &&
            strncmp(e->name, name, len) == 0) {
            e->value = val;
            return;
        }
    }

    /* Not found, append */
    if (env->count == env->capacity) {
        size_t new_cap = env->capacity ? env->capacity * 2 : 4;
        env->entries = realloc(env->entries, new_cap * sizeof(EnvEntry));
        env->capacity = new_cap;
    }

    env->entries[env->count].name = strndup(name, len);
    env->entries[env->count].name_length = len;
    env->entries[env->count].value = val;
    env->count++;
}

bool env_assign(Env* env, const char* name, size_t len, Value val) {
    Env* current = env;
    while (current) {
        for (size_t i = 0; i < current->count; i++) {
            EnvEntry* e = &current->entries[i];
            if (e->name_length == len && strncmp(e->name, name, len) == 0) {
                // Found it! Update it here.
                e->value = val;
                return true; 
            }
        }
        current = current->parent; // Look in the outer scope
    }
    return false; // Variable was never declared
}

/* ================================
   Push a new lexical scope
   ================================ */
Env* env_push(Env* parent) {
    Env* env = malloc(sizeof(Env));
    env->entries = NULL;
    env->count = 0;
    env->capacity = 0;
    env->parent = parent;
    return env;
}

/* ================================
   Pop a lexical scope
   ================================ */
Env* env_pop(Env* env) {
    if (!env) return NULL;

    for (size_t i = 0; i < env->count; i++) {
        // free() the duplicated strings
        free((void*)env->entries[i].name); 
    }
    
    free(env->entries);
    Env* parent = env->parent;
    free(env);
    return parent;
}


