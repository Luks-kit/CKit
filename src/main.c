#include <stdio.h>
#include <stdlib.h>
#include "ast_runtime.h"
#include "eval_context.h"
#include "literal.h"
#include "ident.h"
#include "binop.h"
#include "unop.h"
#include "assign.h"
#include "block.h"
#include "env.h"

int main(void)
{
    Runtime rt;
    runtime_init(&rt);
    ast_register_all(&rt);

    EvalContext ctx = {0};
    ctx.current_env = env_push(NULL);
    ctx.runtime = &rt;

    /* x = 10 */
    IntLiteral* ten = (IntLiteral*)literal_new_int(10); 

    Identifier* x = (Identifier*)identifier_new("x", 1);
    
    Assign* assign_x = (Assign*)assign_new(&x->base, &ten->literal.base);

    /* y = x + 20 */
    IntLiteral* twenty = (IntLiteral*)literal_new_int(20); 

    Identifier* y = (Identifier*)identifier_new("y", 1);   
    
    Identifier* x_again = (Identifier*)identifier_new("x", 1); 

    BinOp* add = (BinOp*)binop_new('+', &x_again->base, &twenty->literal.base);    

    Assign* assign_y = (Assign*)assign_new(&y->base, &add->base);

    /* Block: { x = 10; y = x + 20; } */
    ASTBase* statements[] = {&assign_x->base, &assign_y->base};
    Block* block = (Block*)block_new(statements, 2); 

    Value result = ast_eval(&(block->base), &ctx);
    ast_print(&(block->base), &ctx);
    printf("\nResult type: %d\n", result.type);
    printf("Result: y = %ld\n", result.i);
    
    ast_dtor(&block->base, &ctx);
    env_pop(ctx.current_env);
    return 0;
}
