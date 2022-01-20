#pragma once
#define bfbreak goto _brainfuck_break;

void bf_shl(uint16_t* cell, int repeats);
void bf_shr(uint16_t* cell, int repeats);
uint16_t* bf_optimize(Interpreter* interpret);
int bf_interpreter(Interpreter* interpret);