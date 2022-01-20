#include <stdio.h>
#include <time.h>

#include "brainfuck/brainfuck.h"
#include "file_manager/file.c"

bool arr_find(char* search, char* arr[], int arr_size);
int main(int argc, char* argv[]) {
	if (argc < 2) {
		bf_err("Must specify a file");
		exit(1);
	}

	char* read =  (arr_find("-nf", argv, argc) || arr_find("-nofile", argv, argc)) ? argv[1]: ffullread(argv[1]);
	Interpreter* tokens = bf_tokenize(read);

	bool bytecode_exit = arr_find("-bc", argv, argc) || arr_find("-bytecode", argv, argc);
	bool bytecode_pass = arr_find("-bcp", argv, argc) || arr_find("-bytecodepass", argv, argc);
	if (bytecode_exit || bytecode_pass) {
		if (bytecode_pass && bytecode_exit) bf_err("Only 1 bytecode flag may be used at a time");
		printf("bytecode instructions: ");
		for (int i = 0; i < tokens->operations_size; i++) {
			printf("%dda[%x] ", tokens->operations[i].opcode, tokens->operations[i].repeats);
		}
		puts("");
		if (bytecode_exit) return 0;
	}
	
	float st = 0;
	bool using_benchmark = arr_find("-t", argv, argc) || arr_find("-tm", argv, argc) || arr_find("-time", argv, argc);
	if (using_benchmark) st = (float)clock() / CLOCKS_PER_SEC;
		char last_printed_char = bf_interpreter(tokens);
	if (using_benchmark) if (last_printed_char != 10) printf("\n%lf\n", ((float)clock() / CLOCKS_PER_SEC) - st); else printf("%lf\n", ((float)clock() / CLOCKS_PER_SEC) - st);
	
	free(read);
	return 0;
}

bool arr_find(char* search, char* arr[], int arr_size) {
	for (int i = 0; i < arr_size; i++) {
		if (strcmp(arr[i], search) == 0) {
			return true;
		}
	}
	return false;
}