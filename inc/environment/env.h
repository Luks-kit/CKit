#ifndef ENV_H
#define ENV_H

#include "eval_context.h"

Value env_lookup(Env* env, const char* name, size_t len);
void env_set(Env* env, const char* name, size_t len, Value val);
Env* env_push(Env* parent);
Env* env_pop(Env* env);


#endif
