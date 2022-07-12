#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct stack {
	void **elements;
	ptrdiff_t index;
	ptrdiff_t size;
} stack;

struct stack *create_stack(ptrdiff_t size);

void destroy_stack(struct stack **stk_p);

int stack_push(struct stack *stk, void *element);

void *stack_pop(struct stack *stk);

void *stack_peek(struct stack *stk);

bool stack_is_empty(struct stack *stk);

size_t stack_size(struct stack *stk);

#endif // STACK_H
