#include "lexer.h"

Interpreter* bf_tokenize(char* program) {
	Interpreter* l_interpreter = (Interpreter*)tmalloc(sizeof(Interpreter));
	Lexer l_lexer;

	l_lexer.current_char = 0;
	l_lexer.program = program;
	l_lexer.end_pointer = strlen(l_lexer.program);

	l_interpreter->operations_size = 0;
	l_interpreter->operations_max_size = (1<<7);
	l_interpreter->operations = (Operation*)tmalloc(sizeof(Operation) * l_interpreter->operations_max_size);
	
	int i = 0;
	l_lexer.current_char = l_lexer.program[i];
	while (i <= l_lexer.end_pointer) {
		int op = bf_get_opcode_from_char(l_lexer.current_char);
		while (i <= l_lexer.end_pointer && op == -1) {
			i++;
			l_lexer.current_char = l_lexer.program[i];
			op = bf_get_opcode_from_char(l_lexer.current_char);
		}
		if (i > l_lexer.end_pointer) break;
		
		i++;
		l_lexer.current_char = l_lexer.program[i];

		l_interpreter->operations[l_interpreter->operations_size++] = (Operation){1, op};
		if (l_interpreter->operations_size >= l_interpreter->operations_max_size) {
			l_interpreter->operations_max_size += 16;
			Operation* tmp = (Operation*)trealloc(l_interpreter->operations, sizeof(Operation) * l_interpreter->operations_max_size);
			l_interpreter->operations = tmp;
		}
	}

	int faulty = 0;
	for (int i = 0; i < l_interpreter->operations_size; i++) {
		for (int k = 0; k < sizeof(sequence_list) / sizeof(sequence_list[0]); k++) {
			bool success = true;
			int j = 0;
			int len = sequence_list[k].arr_size;

			while (j < len) {
				if (i + j < l_interpreter->operations_size && l_interpreter->operations[i+j].repeats == 1 && l_interpreter->operations[i+j].opcode == sequence_list[k].seq_tokens[j]) {
					j++;
				} else {
					success = false;
					break;
				}
			}

			if (success) {
				for (int l = 0; l < len; l++) {
					l_interpreter->operations[i + l] = (Operation){0, 0};
					faulty++;
				}
				l_interpreter->operations[i].opcode = sequence_list[k].op;
				l_interpreter->operations[i].repeats = 1;
			}
		}
	}

	Operation operations_cpy[l_interpreter->operations_size - faulty];

	int operations_cpy_size = 0;
	for (int i = 0; i < l_interpreter->operations_size; i++) {
		if (l_interpreter->operations[i].repeats != 0) {
			operations_cpy[operations_cpy_size++] = l_interpreter->operations[i];
		}
	}

	Operation* operations_secondary_cpy = (Operation*)tmalloc(sizeof(Operation) * operations_cpy_size);
	int operations_secondary_cpy_size = 0;
	int j = 0;
	
	while (j < operations_cpy_size) {
		OperationCode initial_op = operations_cpy[j].opcode;
		int reps = 0;
	
		while (operations_cpy[j].opcode == initial_op && j < operations_cpy_size) {
			j++, reps++;
		}
		operations_secondary_cpy[operations_secondary_cpy_size++] = (Operation){reps, initial_op};
	}

	free(l_interpreter->operations);
	l_interpreter->operations = operations_secondary_cpy;
	l_interpreter->operations_size = operations_secondary_cpy_size;

	/*printf("bytecode instructions: ");
	for (int i = 0; i < l_interpreter->operations_size; i++) {
		printf("0x%x%x ", l_interpreter->operations[i].opcode, l_interpreter->operations[i].repeats);
	}
	puts("");*/

	return l_interpreter;
}

int bf_get_opcode_from_char(char character) {
	int found = -1;
	for (int j = 0; j < sizeof(opcode_char_list) / sizeof(opcode_char_list[0]); j++) {
		if (opcode_char_list[j] == character) {
			found = j;
			break;
		}
	}
	return found;
}