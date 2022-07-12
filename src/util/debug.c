#include <stdio.h>

#include <util/debug.h>

void print_ast_helper(struct ast_node *node, size_t depth) {
	for (int i = 0; i < depth; i++) {
		putchar('\t');
	}

	switch(node->type) {
	case INT_LIT_NODE:
		printf("INT: %d\n", node->int_value);
		return;
	case BIN_MSG_NODE:
		printf("BIN MSG: %c\n", node->op);
		print_ast_helper(node->bin_msg_rcvr, depth + 1);
		print_ast_helper(node->bin_msg_arg, depth + 1);
		return;
	}
}

void print_ast(struct ast_node *root) {
	print_ast_helper(root, 0);
}
