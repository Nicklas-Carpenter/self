#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdint.h>

#include <object/object.h> 
#include <util/hash_map.h>

typedef primitive_method (object*)(void *rcvr, void **args);

typedef struct  {
	struct object_descriptor *descriptor;		
	int64_t value;
} int_object 

int init_primitives();

int teardown_primitives();

extern hash_map *primtive_methods;

#endif // PRIMITIVES_H
