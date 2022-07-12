#ifndef INTERNALS_H
#define INTERNALS_H

#include <object/object.h>
#include <object/primitives.h>

int init_object(object *obj);

int_object *create_int_object(int64_t value);

void destroy_object(object *obj);

#endif // INTERNALS_H
