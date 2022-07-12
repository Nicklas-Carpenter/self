#ifndef BYTECODE_H
#define BYTECODE_H

#include <parser/ast.h>
#include <util/list.h>

enum opcode {
	PUSH_LIT,
	PUSH_SELF,
	SEND,
	RESEND
}

typedef struct bytecode {
	enum opcode, 
	int64_t operand;
} bytecode;

struct method {
	char *text;
	list *code;
};

struct method *codegen(struct ast_node *root_expr);

#endif // BYTECODE_H
