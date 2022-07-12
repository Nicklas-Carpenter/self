#ifndef PARSER_H
#define PARSER_H

#define SYMBOL_SIZE_MAX 32 
#define LINE_SIZE_MAX 4096

enum symbol_type {
	INVALID_SYM,
	ID_SYM,
	OP_SYM,
	INT_SYM
};
	
struct symbol {
	enum symbol_type type;
	char str[SYMBOL_SIZE_MAX];		
};

void destroy_symbol(struct symbol **sym_p); 

int parse_chunk(char *chunk);
	
#endif // PARSER_H
