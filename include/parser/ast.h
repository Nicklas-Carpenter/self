#ifndef AST_H
#define AST_H

#include <stdint.h>

#include <parser/parser.h>
#include <util/stack.h>

enum ast_node_type {
	INT_LIT_NODE,
	BIN_MSG_NODE
};

struct ast_node {
	enum ast_node_type type;	

	union {
		char id_str[SYMBOL_SIZE_MAX];
		int64_t int_value;
		struct {
			struct ast_node *bin_msg_rcvr;
			struct ast_node *bin_msg_arg;
			char op;
		};
	};
};

struct ast_node *build_ast(stack *sym_stk, stack *working_stk, stack *expr_stk);

#endif // AST_H 
