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

	bool no_file = arr_find("-nf", argv, argc) || arr_find("-nofile", argv, argc);
	char* read =  (no_file) ? argv[1]: ffullread(argv[1]);
	Interpreter* tokens = bf_tokenize(read);

	bool bytecode_exit = arr_find("-bc", argv, argc) || arr_find("-bytecode", argv, argc);
	bool bytecode_pass = arr_find("-bcp", argv, argc) || arr_find("-bytecodepass", argv, argc);
	if (bytecode_pass && bytecode_exit) bf_err("Only 1 bytecode flag may be used at a time");

	printf("'%s'\n", read);

	if (!bytecode_exit) {
		double st = 0;
		bool using_benchmark = arr_find("-t", argv, argc) || arr_find("-tm", argv, argc) || arr_find("-time", argv, argc);
		if (using_benchmark) st = (double)clock();
		bf_interpreter(tokens);
		if (using_benchmark) printf("\nelapsed time:[%lf]\n", ((double)clock() - st) / (double)CLOCKS_PER_SEC);
	}
	
	if (bytecode_exit || bytecode_pass) {
		for (int i = 0; i < tokens->operations_size; i++) {
			printf("0x%x 0x%x ", tokens->operations[i].opcode, tokens->operations[i].repeats);
		}
		puts("");
	}

	/*
	double st = (double)clock();
	for (int i = 0; i < 10000000; i++) {
		bf_interpreter(tokens);
	}
	printf("\nelapsed time: %.20lf\n", ((double)clock() - st) / (double)CLOCKS_PER_SEC);
	*/

	if (!no_file) free(read);
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