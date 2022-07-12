#include <alloca.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <parser/ast.h>
#include <parser/parser.h>
#include <util/debug.h>
#include <util/stack.h>

#define INITIAL_STACK_SIZE 8

// REVIEW We should be able to build expression as we parse. Look into
// this for later

stack *sym_stk;
stack *working_stk;
stack *expr_stk;

static struct symbol *new_symbol(enum symbol_type type, char *word) {
	struct symbol *new_symbol = malloc(sizeof(struct symbol));

	new_symbol->type = type;
	strncpy(new_symbol->str, word, SYMBOL_SIZE_MAX);

	return new_symbol;
}

void destroy_symbol(struct symbol **sym_p) {
	free(*sym_p);
	*sym_p = NULL;
}
	

// TODO identifiers should also be ended by symbols in addition to spaces
static struct symbol *scan_identifier(char *word) {
	int i = 1;

	while (word[i] != '\0') {	
		char c = word[i];

		if (!isupper(c) && !islower(c) && !isdigit(c) && c != '_') {
			// TODO Add some sort of error handling
			char* bad_sym = alloca(SYMBOL_SIZE_MAX);
			memccpy(bad_sym, word, '\0', SYMBOL_SIZE_MAX);	

			printf("Invald identifier: \"%s\"\n", word);
			return new_symbol(INVALID_SYM, word);
		}
		
		i++;
	}
	
	// DEBUG code
	printf("%s IS an identifier\n\n");

	return new_symbol(ID_SYM, word);
}

// TODO Eventually generalize this to different kinds of numbers
static struct symbol *scan_numeric(char *word) {
	int i = 1;
	
	while (word[i] != '\0') {
		if (!isdigit(word[i])) {
			// TODO Add some sort of error handling here
			char* bad_sym = alloca(SYMBOL_SIZE_MAX);
			printf("Invalid number: \"%s\"\n", word);
			memccpy(bad_sym, word, '\0', SYMBOL_SIZE_MAX);	

			return new_symbol(INVALID_SYM, word);
		}

		i++;
	}

	// DEBUG code
	printf("%s IS is number\n\n");

	return new_symbol(INT_SYM, word);
}	

static int scan_word(char* word) {
	struct symbol *sym;

	char c = word[0];

	// Keyword or identifier
	if (c == '_' || islower(c)) { 
		// TODO Implement the rest of this later
		//if (word[length - 1] == ':') {
		//	sym = parse_small_keyword(word, length - 1);
		//}

		//else {
			// DEBUG code
			printf("%s seems to be an identifier (small or _)\n", word);
			sym = scan_identifier(word);
		//}
	}

	// Identifier
	else if (isupper(c)) {
		// TODO Implement the rest of this later
		//if (word[length - 1] == ':') {
		//	parse_large_keyword(word, length -1);
		//}

		//else {
			// DEBUG code
			printf("%s seems to be an identifier (large)\n", word);

			scan_identifier(word);
		//}
	}

	// Numeric
	else if (c >= '0' && c <= '9') {
		// DEBUG code
		printf("%s seems to be a number\n", word);

		sym = scan_numeric(word);
	}

	// Symbols
	else if (c == '+') {
		// DEBUG code
		printf("%s is a symbol\n\n", word);

		sym = new_symbol(OP_SYM, word);
	}

	// Could not determine type. Input is invalid.
	else  {
		sym = new_symbol(INVALID_SYM, word);
	}

	if (sym->type == INVALID_SYM) {
		// TODO Add some sort of error handling	
		printf("Received invalid symbol \"%s\"\n\n", sym->str);
		return -1;
	}

	stack_push(sym_stk, sym);
	return 0;
}

static int scan_line(char *line) { 
	size_t start = 0; //, end = 0;

	// Skip leading whitespace
	while (isspace(line[start])) {
		start++;
	}

	size_t end = start;

	while (true) {
		if (isspace(line[end]) || line[end] == '\0') {
			if (line[end] != '\0') {
				line[end] = '\0';
				end++;
			}

			int rc = scan_word(line + start);
			if (rc < 0 ) {
				// TODO Throw an error
				printf("Error parsing line \"%s\"\n", line);
			}

			if (line[end] == '\0') {
				return 0;
			}
				
			start = end;
		}

		else {
			end++;	
		}
	}
}

static int scan_chunk(char *chunk) {
	size_t start = 0, end = 0;

	while (true) {
		if (chunk[end] == '\n' || chunk[end] == '\0') {
			if (chunk[end] != '\0') {
				chunk[end] = '\0';
				end++;
			}

			int rc = scan_line(chunk + start);
			if (rc < 0) {
				printf("Error parsing chunk \"%s\"\n", chunk);
				return -1;
			}

			if (chunk[end] == '\0') {
				return 0;
			}			

			start = end;
		}
	
		else {
			end++;
		}
	}
}

int parse_chunk(char *chunk) {
	struct ast_node *top_level_expr;

	sym_stk = create_stack(INITIAL_STACK_SIZE); 
	working_stk = create_stack(INITIAL_STACK_SIZE); 	
	expr_stk = create_stack(INITIAL_STACK_SIZE); 	
	
	scan_chunk(chunk);

	top_level_expr = build_ast(sym_stk, working_stk, expr_stk);
	if (top_level_expr == NULL) {
		// TODO Improve error handling
		puts("Error building AST");
	}
	
	print_ast(top_level_expr);	
	
	return 0;
}
