#include <stdio.h>
#include <string.h>
#include "parser.h" // Assuming this contains your Parser struct and funcs
#include "lexer.h"
#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"
#include "env.h"
#include "call.h"

#define LINE_BUFFER_SIZE 1024

int count_nesting_depth(const char* input) {
    int depth = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '{' || input[i] == '(' || input[i] == '[') {
            depth++;
        } else if (input[i] == '}' || input[i] == ')' || input[i] == ']') {
            depth--;
        }
    }
    return depth;
}

void run_repl(EvalContext* ctx) {
    char line[LINE_BUFFER_SIZE];
    // A larger buffer to accumulate multi-line input
    char total_input[LINE_BUFFER_SIZE * 10] = {0}; 
    int brace_balance = 0;

    printf("CKit REPL (v0.1)\n");
    printf("Press Ctrl+C to exit.\n");

    for (;;) {
        // Change prompt if we are inside a block
        printf(brace_balance > 0 ? ".. " : "> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        // 1. Update brace balance before parsing
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '{') brace_balance++;
            if (line[i] == '}') brace_balance--;
        }

        // 2. Accumulate the input
        strcat(total_input, line);

        // 3. Only evaluate if braces are balanced
        if (brace_balance > 0) {
            continue; 
        }

        if (strlen(total_input) == 0 || strcmp(total_input, "\n") == 0) {
            total_input[0] = '\0';
            continue;
        }
        
        if (strncmp(total_input, "exit", 4) == 0) break;

        // --- Lex and Parse the accumulated buffer ---
        Parser parser;
        lexer_init(&parser.lexer, total_input);
        parser.had_error = false;

        ASTBase* program = parse_program(&parser);

        if (parser.had_error || program == NULL) {
            printf("Parser error, skipping execution\n");
        }
        
        if (!ast_validate(program, ctx) || ctx->has_error) {
            printf("Semantic error: %s\n", ctx->error_message);
            ctx->has_error = false;
        }    
        printf("AST: ");
        ast_print(program, ctx);
        printf("\n");

        Value result = ast_eval(program, ctx);
        printf("Result: ");

        switch (result.type) {
            case (VAL_INT): {printf("%ld\n", result.i); break;}
            case (VAL_FLOAT): {printf("%f\n", result.f); break;}
            case (VAL_BOOL): {printf("%s\n", result.b ? "[true]" : "[false]"); break;}
            case (VAL_STRING): {printf("%.*s\n", (int)result.s.length ,result.s.data); break;}
            case (VAL_NULL): {printf("Null Type(Nothing)\n"); break;}
            case (VAL_FUNCTION): {printf("Function %s\n", type_to_string(result.function->ret_type)); break;}
            default: {printf("Unimplemented type\n"); break; }
        } 
        if (program) ast_dtor(program, ctx);

        // 4. Reset for next command
        memset(&total_input, 0, LINE_BUFFER_SIZE * 10);
        brace_balance = 0; 
    }
}

int main(int argc, char* argv[]) {
    Runtime rt;
    runtime_init(&rt);
    ast_register_all(&rt);
	
    EvalContext ctx = {0};
    ctx.global_env = env_push(NULL);
    ctx.current_env = env_push(ctx.global_env);
    ctx.runtime = &rt;// Assuming you have a constructor
    
    setup_globals(ctx.global_env);

    if (argc == 1)
        run_repl(&ctx); 
    else {
        fprintf(stderr, "Usage: ckit [path]\n");
        return 64;
    }
    
    env_pop(ctx.current_env);

    return 0;
}



