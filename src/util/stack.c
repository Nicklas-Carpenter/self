#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <util/stack.h>

struct stack *create_stack(ptrdiff_t size) {
	struct stack *stk = malloc(sizeof(struct stack));
	if (stk == NULL) {
		// TODO Improve error handling
		puts("Error allocating memory for stack");
		return NULL;
	}

	stk->size = size;
	stk->index = -1;

	stk->elements = malloc(size * sizeof(void *));
	if (stk->elements == NULL) {
		// TODO Improve error handling
		puts("Error allocating stack elements");
		return NULL;
	}

	return stk;
}

void destroy_stack(struct stack **stk_p) {
	free((*stk_p)->elements);
	free(*stk_p);
	*stk_p = NULL;
}

int stack_push(struct stack *stk, void *element) {
	// Check to see if we need to increase the stack size	
	if (++stk->index == stk->size) { 
		size_t new_size = 2 * stk->size * sizeof(void *);
		stk->elements = realloc(stk->elements, new_size);
		if (stk->elements == NULL) {
			// TODO Logging should be integrated into VM,
			// not automatically passed to stdout	
			printf("Error resizing stack: %s\n", strerror(errno));
			return -1;
		}
	}

	stk->elements[stk->index] = element;	
	return 0;
}

size_t stack_size(struct stack *stk) {
	return stk->index + 1;
}

bool stack_is_empty(struct stack *stk) {
	return stk->index == -1;
}

void *stack_peek(struct stack *stk) {
	if (stack_is_empty(stk)) {
		return NULL;
	}

	return stk->elements[stk->index];
}

void *stack_pop(struct stack *stk) {
	if (stack_is_empty(stk)) {
		 return NULL;
	}

	return stk->elements[stk->index--];
}
