#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#include "brainfuck.h"
#include "bytecode/bytecode.h"
#include "lexer/lexer.c"
#include "interpreter/interpreter.c"

int bf_err(const char* methamphetamine) {
	fprintf(stderr, "Error while executing brainfuck: \e[4m%s\e[0m\n", methamphetamine);
	exit(EXIT_FAILURE);

	return 1;
}

void* tmalloc(size_t size) {
	void* m = (void*)malloc(size);
	if (!m) bf_err("Failed to malloc");
	return m;
}

void* trealloc(void* buf, size_t size) {
	void* m = (void*)realloc(buf, size);
	if (!m) bf_err("Failed to realloc");
	return m;
}