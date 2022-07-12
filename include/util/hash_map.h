/*
 * A simple implementation of a string hash map utilizing a polynomial
 * rolling hash function and linear probing. This is a simple and first
 * first implementation. In the future it would be good to try to find
 * more efficient implementations  
 *
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h>
#include <stdint.h>

/*
 * Parameters for the hashing function:
 * 	
 * 	H(s) = (s[0] + s[1] * P + s[2] * P^2 + ... + s[n - 1] P^(n-1)) % M 
 *	
 * Where:
 *	H is the hashing function
 *	P is a prime number
	M is the upper limit of the hash
	s is a string input
	n is the length of the string s	
 *
 */

// Closest prime to the number of printable ASCII characters
#define HASH_P 97 		

// Close to max 64 bit unsigned (which we avoid to reduce probability
// of collisions while still having a high limit)
#define HASH_M ((uint64_t)1 << 63) 


/* Hash map parameters  */
#define MAP_INITIAL_LENGTH 	16 
#define MAP_RESIZE_FACTOR   	 2

struct entry {
	uint64_t hash;
	void *element;
};

typedef struct hash_map {
	ptrdiff_t allocated;
	ptrdiff_t size;	
	struct entry *entries;
} hash_map;

hash_map *create_hash_map();

void destroy_hash_map(hash_map **map_p);

int hash_map_insert(hash_map *map, char *key, void *element);

int hash_map_remove(hash_map *map, char *key);

void *hash_map_get(hash_map *map, char *key);

#endif // HASH_MAP_H
