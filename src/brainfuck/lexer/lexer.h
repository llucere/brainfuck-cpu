#pragma once



typedef struct {
	size_t pointer, end_pointer;
	char current_char, *program;
} Lexer;

Interpreter* bf_tokenize(char* program);
int bf_get_opcode_from_char(char character);
void bf_rm_str(size_t ind, char** str);
void bf_clean(Lexer* l_lexer);