#pragma once
#define bfbreak goto _brainfuck_break;

uint16_t* bf_optimize(Interpreter* interpret);
int bf_interpreter(Interpreter* interpret);