cmake_minimum_required(VERSION 3.10)
project(CKit C)

include_directories(inc/)

add_executable(CKit
	src/ast/assign.c
	src/ast/ast_runtime.c
	src/ast/binop.c
	src/ast/block.c
	src/ast/call.c
	src/ast/ident.c
	src/ast/literal.c
	src/ast/unop.c
	src/eval/env.c
	src/register.c
	src/main.c
	)	
