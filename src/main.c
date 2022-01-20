/*----------------------------------------------------------------*\
	Current Commands:
		default
		+	ADD
		-	SUB
		>	SHR
		<	SHL
		[	LOP
		]	ELP
		,	CHR	,,,,	MSTR	(multi-string)
		.	PUT

		custom
		{	COND
		}	ECND
		#	DRAW	(cur cell x, next cell y, next next cell color)
		$	EXIT
		%	NOP
		*	CLS	(clear screen)

		[-]	CLRS	(clear)
		@	GOTO a	(set current cell to a)
		@@	GOTO A	(set current cell to A)
		/	LINE FEED (outputs 10 ascii)

	Ideas:
		'Hide' command:
			- Hides the cells curent value during the next 2 cycles, e.g.
			```
			mem[0] = 65;
			hide(&mem[0]);
			. ; does not print A
			. ; does not print A
			. ; prints A
			- Gets refreshed once the hide command is called upon again

		Multi-threading:
			- () creates multiple 'threads' (concurrency), with ! being the command to break out the current thread e.g.
			```
			++(%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@.)@@.
			output:
			Aa (with % being a nop call which temporarily 
			pauses the current thread for a few clock cycles, 
			@@ being the cmd to print A, and @ being the cmd to print a)

/*----------------------------------------------------------------*/

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
			printf("0x%x[%x] ", tokens->operations[i].opcode, tokens->operations[i].repeats);
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