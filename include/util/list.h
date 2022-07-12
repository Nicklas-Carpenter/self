#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

#define LIST_RESIZE_FACTOR 2
	
typedef struct {
	ptrdiff_t allocated;
	ptrdiff_t idx;
	void **elements;	
} list;

list *create_list(size_t size)

void list_destroy(list **lst_p);

int list_add(list *lst, void *element);

int list_insert(list *lst, size_t pos, void *element);

void *list_replace(list *lst, size_t pos, void *element);

void *list_delete(list *lst);

void *list_remove(list *lst, size_t pos);

void *list_get(list *lst, size_t pos);

size_t list_size(list *lst);

bool list_is_empty(list *lst);

#endif  // LIST_H
