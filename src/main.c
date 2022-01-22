#include <stdio.h>
#include <time.h>

#include "brainfuck/brainfuck.h"
#include "file_manager/file.c"

int arr_find(char* search[], char* arr[], int arr_size, int search_size, bool ind);
int main(int argc, char* argv[]) {
	if (argc < 2) {
		bf_err("Must specify a file");
		exit(1);
	}
	
	bool no_file = arr_find((char*[2]){"-nf", "-nofile"}, argv, argc, 2, false);
	char* read = no_file ? argv[1]: ffullread(argv[1]);
	Interpreter* tokens = bf_tokenize(read);

	bool bytecode_exit = arr_find((char*[2]){"-bc", "-bytecode"}, argv, argc, 2, false);
	bool bytecode_pass = arr_find((char*[2]){"-bcp", "-bytecodepass"}, argv, argc, 2, false);
	if (bytecode_pass && bytecode_exit) bf_err("Only 1 bytecode flag may be used at a time");

	int exec_rep = arr_find((char*[3]){"-r", "-rep", "-repeat"}, argv, argc, 3, true);
	if (bytecode_exit == false) {
		double st = 0;
		int view_time = arr_find((char*[3]){"-t", "-tm", "-time"}, argv, argc, 3, false);
		
		if (exec_rep == -1) {
			if (view_time) st = (double)clock();
			bf_interpreter(tokens);
			if (view_time) printf("\nelapsed time:[%lf]\n", ((double)clock() - st) / (double)CLOCKS_PER_SEC);
		} else {
			int rep = exec_rep + 1;
			if (!argv[rep]) {
				bf_err("Invalid flag '-repeat' / '-rep', no integer value passed after it");
			}

			char* intv = argv[rep];
			int sum = 0;

			while (*intv && '0' <= *intv <= '9') {
				sum = (sum*10) + (*(intv++) - '0');
			}
			
			if (*intv != '\0') {
				printf("(unexpected '%c' in '%s')\n", *intv, intv);
				bf_err("Invalid flag argument for -repeat / -rep. Did you specify an integer after the flag?");
			}

			if (view_time) st = (double)clock();
			for (int i = 0; i < sum; i++) {
				bf_interpreter(tokens);
			}
			if (view_time) printf("\nelapsed time:[%lf]\n", ((double)clock() - st) / (double)CLOCKS_PER_SEC);
		}
	}

	if (bytecode_exit || bytecode_pass) {
		for (int i = 0; i < tokens->operations_size; i++) {
			printf("0x%x 0x%x ", tokens->operations[i].opcode, tokens->operations[i].repeats);
		}
		puts("");
	}

	if (!no_file) free(read);
	return 0;
}

int arr_find(char* search[], char* arr[], int arr_size, int search_size, bool ind) {
	for (int i = 0; i < arr_size; i++) {
		for (int j = 0; j < search_size; j++) {
			if (strcmp(arr[i], search[j]) == 0) {
				return ind ? i : true;
			}
		}
	}
	return ind ? -1 : false;
}