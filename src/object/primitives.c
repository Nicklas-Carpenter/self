#include <stdio.h>

#include <object/primitives.h>

object *_int_add(object* rcvr, object **args) {						
	if (rcvr->descriptor.type != INT_OBJECT) {
		// TODO Implement better error handling		
		puts("IntAdd: receiver must be an int object");
		return NULL;
	} 

	int_object *arg = (int_object *)args[0];
	if (arg == NULL || arg->descriptor.type != INT_OBJECT) {
		puts("IntAdd: argument is missing or invalid");
	}

	int64_t value = ((int_object *)rcvr)->value + arg->value;
	int_object *result = create_int_object(value);
}

int init_primitives() {
	hash_map *prims = create_hash_map();	
	
	hash_map_insert("IntAdd", _int_add);
}
