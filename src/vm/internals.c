#include <stdio.h>
#include <stdlib.h>

#include <vm/internals.h>

int_object *create_int_object(int64_t value) {
	int_object *new_int = malloc(sizeof(int_object));
	if (new_int == NULL) {
		goto memory_error;
	}

	new_int->descriptor = malloc(sizeof(struct object_descriptor));
	if (new_int->descriptor == NULL) {
		goto memory_error;
	}

	new_int->descriptor->type = INT_OBJECT;
	new_int->descriptor->primitive_methods = primitive_methods;
	new_int->descriptor->slots = NULL;

	new_int->value = value;

	return new_int;
	
	memory_error:
	// TODO Improve error handling
	puts("Error allocating space for new int");	
	return NULL;
}
