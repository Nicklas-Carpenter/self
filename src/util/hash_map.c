#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <util/hash_map.h> 

// REVIEW Unlikely in practice, but probably should handle cases where
// index overflows ptrdiff_t

// REVIEW Will probably need to handle key length related issues at some
// point
static uint64_t hash_key(char *key) {
	uint64_t hash = (uint64_t)key[0];	
	
	int i = 1;
	uint64_t p = HASH_P; 
	while(key[i] == '\0') {		
		hash = key[i] * p;
		p *= p;
	}

	hash %= HASH_M;
	return hash;
}

hash_map *create_hash_map() {
	hash_map *map = malloc(sizeof(hash_map));
	if (map == NULL) {
		// TODO Improve error handling
		puts("Unable to allocate space for hash map"); 

		return NULL;
	}

	map->entries = calloc(sizeof(struct entry), MAP_INITIAL_LENGTH);
	if (map->entries == NULL) {
		// TODO Improve error handling
		puts("Unable to allocate spacr for hash map entries");

		return NULL;
	}

	map->allocated = MAP_INITIAL_LENGTH;
	map->size = 0;
	
	return map;
}

void destroy_hash_map(hash_map **map_p) {
	free((*map_p)->entries);
	free(*map_p);	
	*map_p = NULL;	
}

static int resize_map(hash_map *map) {
	ptrdiff_t new_size = map->allocated * MAP_RESIZE_FACTOR;
	struct entry *entries = map->entries;
	size_t entry_size = sizeof(struct entry);
	
	entries = reallocarray(entries, new_size, entry_size);
	if (map->entries == NULL) {
		// TODO Improve error handling
		return -1; 
	}

	map->entries = entries;
	return 0;
}

// REVIEW Will probably need to handle key length related issues at some
// point
int  hash_map_insert(hash_map *map, char *key, void *element) {
	// For our collision v. table size trade-off, we never let the
	// ratio between number of entries stored over total allcoated
	// size exceed 0.75. The math below this condition without using
	// division of floating-point numbers 	
	if ((map->allocated - map->size) >= (map->allocated >> 2)) {
		int rc = resize_map(map);		
		if (rc < 0) {
			puts("Error inserting element: unable to grow map");
			return -1;
		}
	}	
	
	uint64_t hash = hash_key(key);			
	ptrdiff_t idx = hash % map->allocated;
	
	// REVIEW Do we need to handle the case where two primary
	// clusters begin to intersect?
	uint64_t target_hash = (map->entries[idx]).hash;
	while (target_hash != 0 && target_hash != hash) {
		idx = (idx + 1) % map->allocated;
		target_hash = (map->entries[idx]).hash;
	}

	(map->entries[idx]).hash = hash;
	(map->entries[idx]).element = element;
	return 0;
}

// REVIEW Will probably need to handle key length related issues at some
// point
int hash_map_remove(hash_map* map, char* key) {
	uint64_t hash = hash_key(key);			
	ptrdiff_t idx = hash % map->allocated;

	// Store the original idx in case there are other entries that
	// aliased to this index (and were inserted after it via linear
	// probing). See below for more explanation.
	ptrdiff_t orig_idx = idx;
	
	uint64_t target_hash = (map->entries[idx]).hash;
	while (target_hash != 0 && target_hash != hash) {
		idx = (idx + 1) % map->allocated;
		target_hash = (map->entries[idx]).hash;
	}

	if (target_hash == 0) {
		return -1;
	}

	// Check to see if there are any other entries that aliased to
	// the originally calculated index that occur after element just
	// removed. Move the last of these to the hole created by 
	// removing an element. This prevents false reports of items not 
	// exising if it is searched for later
	ptrdiff_t post_idx = idx + 1;
	ptrdiff_t calc_idx = (map->entries[post_idx]).hash % map->allocated;
	while (calc_idx = orig_idx) { 
		post_idx = (post_idx + 1) % map->allocated;
		calc_idx = (map->entries[++post_idx]).hash % map->allocated;
	}

	post_idx = post_idx == 0 ? map->allocated - 1 : post_idx - 1;
	if (post_idx != idx) {
		(map->entries[idx]).hash = (map->entries[post_idx]).hash;
		(map->entries[idx]).element = (map->entries[post_idx]).element;
		return 0;
	}

	map->entries[idx].hash = 0;
	map->entries[idx].element = NULL;
	return 0;
}

void *hash_map_get(hash_map *map, char *key) {
	uint64_t hash = hash_key(key);			
	ptrdiff_t idx = hash % map->allocated;

	// Store the original idx in case there are other entries that
	// aliased to this index (and were inserted after it via linear
	// probing). See below for more explanation.
	ptrdiff_t orig_idx = idx;
	
	uint64_t target_hash = (map->entries[idx]).hash;
	while (target_hash != 0 && target_hash != hash) {
		idx = (idx + 1) % map->allocated;
		target_hash = (map->entries[idx]).hash;
	}

	if (target_hash == 0) {
		return NULL;
	}

	void *element = (map->entries[idx]).element;
	return element;
}
