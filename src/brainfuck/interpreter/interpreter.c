#include "interpreter.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>

	void slp(int s) {Sleep(s);}
	void clear_screen() {printf("\e[1;1H\e[2J");} // printf("\e[1;1H\e[2J") is faster than system("cls")
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
	#include <unistd.h>
	void clear_screen() {printf("\e[1;1H\e[2J");}
	void slp(int s) {usleep(s*1000);}
#endif

uint16_t* bf_optimize(Interpreter* interpret) {
	uint16_t* loop_points = (uint16_t*)tmalloc(interpret->operations_size * sizeof(uint16_t));
	if (loop_points == NULL) bf_err("Failed to malloc");

	uint8_t lop_stack[interpret->operations_size], cond_stack[interpret->operations_size];
	uint32_t lop_stack_size = 0, cnd_stack_size = 0;

	int start;
	OperationCode op;
	for (int i = 0; i < interpret->operations_size; i++) {
		op = interpret->operations[i].opcode;
		if (op == LOP) {
			lop_stack[lop_stack_size++] = i;
		} else if (op == ELP) {
			start = lop_stack[--lop_stack_size];
			loop_points[i] = start;
			loop_points[start] = i;
			lop_stack[lop_stack_size] = 0;
		} else if (op == COND) {
			cond_stack[cnd_stack_size++] = i;
		} else if (op == ECND) {
			start = cond_stack[--cnd_stack_size];
			loop_points[i] = start;
			loop_points[start] = i;
			cond_stack[cnd_stack_size] = 0;
		} else {
			loop_points[i] = 0;
		}
	}
	
	if (lop_stack_size != 0 || cnd_stack_size != 0) bf_err("Unbalanced loops");

	return loop_points;
}

int bf_interpreter(Interpreter* interpret) {
	__label__ _brainfuck_break;

	uint16_t* loop_map = bf_optimize(interpret);
	register uint16_t cell = 0;
	uint8_t memory[30000];
	register int res, pointer = 0, max_pointer = interpret->operations_size;

	Operation data;
	while (pointer <= max_pointer) {
		data = interpret->operations[pointer];

		switch (data.opcode) {
			case ADD: memory[cell] = (res = memory[cell] + data.repeats) < 0x100 ? res : (res & UCHAR_MAX + UCHAR_MAX) & UCHAR_MAX;
				break;
			case SUB: memory[cell] = (res = memory[cell] - data.repeats) > -1 ? res : (res & UCHAR_MAX + UCHAR_MAX) & UCHAR_MAX;
				break;
			case CLRS: memory[cell] = 0;
				break;
			case SHR:
				if ((res = cell + data.repeats) < 30001) {
					cell = res;
				} else {
					bf_err("Out of boundaries (data pointer exceeded limit (30,001))");
				}
				break;
			case SHL:
				if ((res = cell - data.repeats) > -1) {
					cell = res;
				} else {
					bf_err("Out of boundaries (data pointer preceeded 0)");
				}
				break;
			case LOP: if (memory[cell] == 0) pointer = loop_map[pointer];
				break;
			case ELP:
			case COND: if (memory[cell] != 0) pointer = loop_map[pointer];
				break;
			case PUT: fputchar(memory[cell]);
				break;
			case SLP: slp(100*data.repeats);
				break;
			case DRAW: break;
			case CLS: clear_screen();
				break;
			case CHR:
				if (data.repeats == 1) {
					fflush(stdin);
					memory[cell] = fgetchar();
				} else {
					res = data.repeats + 1;
					uint8_t buf[res];
					fflush(stdin);
					fgets(buf, res, stdin);
					memcpy(&memory[0] + cell, buf, strlen(buf));
				}
				
				break;
			case LNFD: fputchar(10);
				break;
			case SETA: memory[cell] = 'a';
				break;
			case STDA: memory[cell] = 'A';
				break;
			case EXIT: bfbreak;
			case ECND: break;
		}

		pointer++;
	}

	_brainfuck_break:
	
	return 0;
}