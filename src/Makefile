CC=gcc

CFLAGS='-g'
CPPFLAGS='-I../include'

.PHONY: all
all: self

.PHONY: clean
clean:
	rm *.o */*.o

self: self.o parser.o util/stack.o ast.o util/debug.o util/hash_map.o

