#include <stdio.h>
#include <stdlib.h>

#include <vm/bytecode.h>

#define INITIAL_BYTECODE_LIST_SIZE

struct method *create_method() {
	struct method *method = malloc(sizeof(struct method));
	if (method == NULL) {
		// TODO Improve error handling 
		put("Failed to allocate space for struct method pointer");
	}

	// TODO Add the functionality to include source text with the
	// method's bytecode
	// #method->text = 

	method->list = create_list(INITIAL_BYTECODE_LIST_SIZE);
}

struct method *codegen(struct ast_node *root_expr) {
	
}
