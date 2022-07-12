#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>

#include <util/hash_map.h>

#define INITIAL_SLOT_LIST_SIZE 8

enum slot_type {
	DATA_SLOT,
	ASSIGNMENT_SLOT,
	METHOD_SLOT
};

struct slot_descriptor {
	enum slot_type type;	
	ptrdiff_t offset;	
};

enum object_type {
	INT_OBJECT,
	PRIMITIVE_METHOD_OBJECT
	DATA_OBJECT,
	METHOD_OBJECT,
	BLOCK_OBJECT
}

struct object_descriptor {
	enum object_type type;
	hash_map *primitive_methods;
	hash_map *slots;
};

typedef struct object object;
struct object {	
	struct object_descriptor *descriptor;	
	object *slots;
	
} object;

#endif // OBJECT_H
