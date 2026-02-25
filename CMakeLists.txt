cmake_minimum_required(VERSION 3.10)
project(CKit C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)


include_directories(inc/ inc/ast/ inc/environment/ inc/parse)



add_executable(CKit
	src/ast/assign.c
	src/ast/ast_runtime.c
	src/ast/binop.c
	src/ast/block.c
	src/ast/call.c
	src/ast/ident.c
	src/ast/literal.c
	src/ast/unop.c
    src/ast/decl.c
    src/ast/if.c
    src/ast/while.c
    src/ast/fndecl.c
	src/eval/env.c
    src/register.c
	src/main.c
	src/parse/lexer.c
	src/parse/parse_expr.c
	src/parse/parse_stmt.c
	src/parse/parse_utils.c
)	
