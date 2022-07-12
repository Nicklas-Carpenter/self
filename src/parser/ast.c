#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include <object/primitives.h>
#include <parser/ast.h>
#include <vm/bytecode.h>
#include <vm/internals.h>

/*
 * Create an integer literal AST node
 *
 * Parameters:
 *	int_str - The string form of the integer value
 *
 * Returns:
 *	node - The newly constructed integer literal AST node on success
 *	NULL - If there is an error constructing the node 
 *
 */
static struct ast_node *create_int_lit_node(char *int_str) {
	struct ast_node *node = malloc(sizeof(struct ast_node));
	if (node == NULL) {
		// TODO Improve error handling
		puts("Error allocating memory of integer literal AST node");
		return NULL;
	}	

	node->type = INT_LIT_NODE;

	errno = 0;
	node->int_value = (int)strtol(int_str, NULL, 10);		

	// Unlikely, but if it happens, the error will be very hard to
	// detect since strtol will return 0 (a valid int value)
	if (errno == EINVAL) {
		printf("Error converting \"%s\" to int value\n", int_str);
		return NULL;
	}
	
	return node;	
}

/* 
 * Creates a binary message AST node
 * 
 * Parameters:
 * 	op   - The binary message operator
 * 	rcvr - The receiver of the binary message
 * 	arg  - The argument to the binary message 	
 *
 * Returns:
 * 	node - The newly-constructed binary message node on success
 * 	NULL - If an error occurs while constructing the node 
 *
 */
static struct ast_node *create_bin_msg_node(
	struct ast_node *rcvr,
	struct ast_node *arg,
	char op
) {
	struct ast_node *node = malloc(sizeof(struct ast_node));
	if (node == NULL) {
		// TODO Improve error handling
		puts("Error allocating memory of integer literal AST node");
		return NULL;
	}

	node->type = BIN_MSG_NODE;
	
	node->op = op;
	node->bin_msg_rcvr = rcvr;
	node->bin_msg_arg = arg;

	return node;
}

void destroy_ast_node(struct ast_node **node_p) {
	struct ast_node *node = *node_p;

	switch(node->type) {
	case INT_LIT_NODE:
		break;
	case BIN_MSG_NODE:
		destroy_ast_node(&node->bin_msg_rcvr);
		destroy_ast_node(&node->bin_msg_arg);
		break;	
	}

	free(node);			
	*node_p = NULL;
}

int codegen_int_lit_node(struct ast_node *node, bytecode *ops) {
	int_object *int_obj = create_int_object(node->int_value);
}

struct ast_node *build_ast(stack *sym_stk, stack *working_stk, stack *expr_stk) {
	while (!stack_is_empty(sym_stk)) {
		struct symbol *cur = stack_pop(sym_stk);
		switch (cur->type) {
		case INT_SYM:
			struct ast_node *int_lit;
			int_lit = create_int_lit_node(cur->str);				 
			destroy_symbol(&cur);

			stack_push(expr_stk, int_lit);
			break;
		case OP_SYM:
			stack_push(working_stk, cur); 
			break;
		}	
	}

	while (!stack_is_empty(working_stk)) {
		struct symbol *cur = stack_pop(working_stk);	
		switch(cur->type) {
		case OP_SYM:
			struct ast_node *rcvr, *arg;	

			rcvr = stack_pop(expr_stk);
			if (rcvr == NULL) {
				// TODO Improve error handling
				puts("Error getting rcvr building bin msg: stack empty");
				return NULL;
			}

			arg = stack_pop(expr_stk);
			if (arg == NULL) {
				puts("Error getting arg building bin msg: stack empty");
				return NULL;
			}

			char op = cur->str[0];

			struct ast_node *bin_msg;
			bin_msg = create_bin_msg_node(rcvr, arg, op);
	
			stack_push(expr_stk, bin_msg);
			break;
		}
	}	
	
	// Each chunk parsed should only have one top-level expression
	// since the AST can only have one root node
	if (stack_size(expr_stk) > 1) {
		// TODO Improve error handling	
		puts("Error building AST: more than one top-level expression");	
		return NULL;
	}

	return stack_pop(expr_stk);
}
