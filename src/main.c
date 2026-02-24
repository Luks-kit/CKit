#include <stdio.h>
#include <string.h>
#include "parser.h" // Assuming this contains your Parser struct and funcs
#include "lexer.h"
#include "ast.h"
#include "ast_runtime.h"
#include "eval_context.h"
#include "env.h"

#define LINE_BUFFER_SIZE 1024

void run_repl(EvalContext* ctx) {
    char line[LINE_BUFFER_SIZE];

    printf("CKit REPL (v0.1)\n");
    printf("Press Ctrl+C to exit.\n");

    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        // Handle empty input
	// 1. Strip the newline so the Lexer hits '\0' (EOF) immediately after the code
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) continue;
        if (strcmp(line, "exit") == 0) break;


        // Initialize components
        Parser parser;
        lexer_init(&parser.lexer, line);
        parser.had_error = false;
        
        // Use your parse_program entry point
        ASTBase* program = parse_program(&parser);

        if (!parser.had_error && program != NULL) {
            // Optional: Print the tree for debugging
            printf("AST: ");
	        ast_print(program, ctx);
            printf("\n");

            // Evaluate the program
            Value result = ast_eval(program, ctx);
            printf("Result: ");
            
            switch (result.type) {
                case (VAL_INT): {printf("%ld\n", result.i); break;}
                case (VAL_FLOAT): {printf("%f\n", result.f); break;}
                case (VAL_BOOL): {printf("%s\n", result.b ? "[true]" : "[false]"); break;}
                case (VAL_STRING): {printf("%.*s\n", (int)result.s.length ,result.s.data); break;}
                default: {printf("Unimplemented type\n"); break; }
            }

        }

        // Clean up the AST for this line
        if (program) {
            ast_dtor(program, ctx);
        }
    }
}

int main(int argc, char* argv[]) {
    Runtime rt;
    runtime_init(&rt);
    ast_register_all(&rt);
	
    EvalContext ctx = {0};
    ctx.current_env = env_push(NULL);
    ctx.runtime = &rt;// Assuming you have a constructor
    
    if (argc == 1) {
        // Run REPL
        run_repl(&ctx);
    } else if (argc == 2) {
        // Run File (Optional functionality)
        // char* source = read_file(argv[1]);
        // ... parse and eval ...
    } else {
        fprintf(stderr, "Usage: ckit [path]\n");
        return 64;
    }
    
    env_pop(ctx.current_env);

    return 0;
}



