#include <stdio.h>
#include <stdlib.h>

#include <util/list.h>

list *create_list(size_t size) {
	list *new_list = malloc(sizeof(list));
	if (new_list == NULL) {
		// TODO Improve error handling
		puts("Unable to space for list pointer");
		return NULL;
	}

	new_list->elements = calloc(size, sizeof(void*));
	if (new_list->elements == NULL) {
		// TODO Improve error handling
		printf("Unable to allocate space for list of size %d", size);
	}

	new_list->allocated = size;
	new_list->idx = 0;

	return list
}

void destroy_list(list **list_p) {
	free((*list_p)->elements);
	free(*list_p);
	*list_p = NULL;
}

static int grow_list(list *lst, size_t size) {
	void **elements = lst->elements;

	elements = reallocarray(elements, size, sizeof(void *)); 
	if (elements == NULL) {
		// TODO Improve error handling
		return -1;
	}

	lst->elements = elements;	
	lst->allocated = size;
	return 0;
}

int list_add(list *lst, void *element) {
	if (lst->idx == lst->allocated) {
		size_t new_size = list->allocated * LIST_RESIZE_FACTOR;
		int rc = grow_list(lst, new_size);
		if (rc < 0) {
			// TODO Improve error handling
			printf("Unable to grow list to size %d", new_size);
			return -1;
		}
	}

	lst->elements[list->idx++] = element;	
	return 0;
}

int list_insert(list *lst, size_t pos, void *element) {\
	if (pos >= lst->idx) {
		// TODO Improve error handling
		puts("Error: tried to remove element past end of list");
		return -1;
	}

	if (lst->idx == lst->allocated) {
		size_t new_size = list->allocated * LIST_RESIZE_FACTOR;
		int rc = grow_list(lst, new_size);
		if (rc < 0) {
			// TODO Improve error handling
			printf("Unable to grow list to size %d", new_size);
			return -1;
		}
	}

	if (list->elements[pos] != NULL) {
		for (int i = list->idx; i >= pos; i--) {
			list->elements[i + 1] = list->elements[i];
		} 	
	}	

	list->elements[pos] = element;
	list->idx++;
	return 0;	
}

void *list_replace(list *lst, size_t pos, void *element) {
	if (pos >= lst->idx) {
		// TODO Improve error handling
		puts("Error: tried to remove element past end of list");
		return NULL;
	}

	void* element = lst->elements[pos];
	lst->elements[pos] = NULL;
	
	return element;
} 

void *list_remove(list *lst) {
	if (lst->idx == 0) {
		return NULL;
	}	
	
	void *element = lst->elements[--lst->idx];
	lst->elements[lst->idx];

	return element;
}

void *list_delete(list *lst, size_t pos) {
	if (pos >= lst->idx) {
		// TODO Improve error handling
		puts("Error: tried to remove element past end of list");
		return NULL;
	}
	
	void *element = lst->elements[pos];
	
	if (pos < list->idx - 1) {
		for (int i = pos; i < list_idx - 1; i++) {
			lst->elements[i] = lst->elements[i + 1];
		}	
	}

	lst->elements[--lst->idx] = NULL;	
	return element;
}

void *list_get(list *lst, size_t pos) {
	if (pos >= lst->idx) {
		// TODO Improve error handling
		puts("Error: tried to remove element past end of list");
		return NULL;
	}

	return  list->elements[pos];	
}

size_t list_size(list *lst) {
	return lst->idx;
}
	
size_t list_is_empty(list *lst) {
	return lst->idx == 0;
} 
