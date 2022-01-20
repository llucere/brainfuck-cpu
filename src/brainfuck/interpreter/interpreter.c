#include "interpreter.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>

	void slp(int s) {Sleep(s);}
	void clear_screen() {printf("\e[1;1H\e[2J");} // printf("\e[1;1H\e[2J") is faster than system("cls")
#else
	#ifdef defined(__unix__) || defined(__linux__) || defined(__APPLE__)
		#include <unistd.h>
		void clear_screen() {printf("\e[1;1H\e[2J");}
		void slp(int s) {usleep(s*1000);}
	#endif
#endif

int mod(int x, int m) {
	// https://stackoverflow.com/a/16964329 but edited
	return (x & m + m) & m;
}

uint16_t* bf_optimize(Interpreter* interpret) {
	uint16_t* loop_points = (uint16_t*)tmalloc(interpret->operations_size * sizeof(uint16_t));
	if (loop_points == NULL) bf_err("Failed to malloc");

	uint8_t lop_stack[interpret->operations_size], cond_stack[interpret->operations_size];
	uint32_t lop_stack_size = 0, cnd_stack_size = 0;

	for (int i = 0; i < interpret->operations_size; i++) {
		if (interpret->operations[i].opcode == LOP) {
			lop_stack[lop_stack_size++] = i;

		} else if (interpret->operations[i].opcode == ELP) {
			int start = lop_stack[lop_stack_size-1];
			loop_points[i] = start;
			loop_points[start] = i;
			lop_stack[--lop_stack_size] = 0;

		} else if (interpret->operations[i].opcode == COND) {
			cond_stack[cnd_stack_size++] = i;

		} else if (interpret->operations[i].opcode == ECND) {
			int start = cond_stack[cnd_stack_size-1];
			loop_points[i] = start;
			loop_points[start] = i;
			cond_stack[--cnd_stack_size] = 0;

		} else {
			loop_points[i] = 0;
		}
	}
	
	if (lop_stack_size != 0 && cnd_stack_size != 0) bf_err("Unbalanced loops");

	return loop_points;
}

int bf_interpreter(Interpreter* interpret) {
	__label__ _brainfuck_break;

	register size_t pointer = 0, max_pointer = interpret->operations_size;

	uint8_t last_printed_char;
	uint16_t* loop_map = bf_optimize(interpret);
	uint16_t cell = interpret->cell, memory[USHRT_MAX];

	Operation data;
	while (pointer <= max_pointer) {
		data = interpret->operations[pointer];

		switch (data.opcode) {
			case ADD:
				if (memory[cell] + data.repeats < 0x100) {
					memory[cell] += data.repeats;
				} else {
					memory[cell] = mod(memory[cell] + data.repeats, UCHAR_MAX);
				}
				break;
			case SUB:
				if (memory[cell] - data.repeats > -1) {
					memory[cell] -= data.repeats;
				} else {
					memory[cell] = mod(memory[cell] - data.repeats, UCHAR_MAX);
				}
				break;
			case SLP:
				slp(100*data.repeats);
				break;
			case DRAW:
				break;
			case CLS:
				clear_screen();
				break;
			case CLRS:
				memory[cell] = 0;
				break;
			case SHR:
				if (cell + data.repeats > USHRT_MAX) {
					bf_err("Out of boundaries (data pointer exceeded 2^16)");
				} else {
					cell += data.repeats;
				}
				interpret->cell = cell;
				break;
			case SHL:
				if (cell - data.repeats < 0) {
					bf_err("Out of boundaries (data pointer preceeded 0)");
				} else {
					cell -= data.repeats;
				}
				interpret->cell = cell;
				break;
			case LOP:
				if (memory[cell] == 0) pointer = loop_map[pointer];
				break;
			case ELP:
				if (memory[cell] != 0) pointer = loop_map[pointer];
				break;
			case COND:
				if (memory[cell] != 0) pointer = loop_map[pointer];
				break;
			case CHR:
				if (data.repeats == 1) {
					fflush(stdin);
					memory[cell] = fgetc(stdin);
				} else {
					uint8_t buf[data.repeats + 1];
					fflush(stdin);
					fgets(buf, data.repeats + 1, stdin);

					for (int i = 0; i < strlen(buf); i++) {
						memory[cell + i] = buf[i];
					}
				}
				
				break;
			case LNFD:
				fputchar(10);
				break;
			case PUT:
				fputchar(memory[cell]);
				last_printed_char = memory[cell];
				break;
			case SETA:
				memory[cell] = 'a';
				break;
			case STDA:
				memory[cell] = 'A';
				break;
			case EXIT:
				bfbreak;
			case ECND: break;
		}

		pointer++;
	}

	_brainfuck_break:
	
	return last_printed_char;
}