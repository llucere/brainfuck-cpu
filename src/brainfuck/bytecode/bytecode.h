#pragma once


// opcodes and opcode_char_list must be ordered in the exact same way.

typedef enum op_code_t {
	ADD,
	SUB,
	SHR,
	SHL,
	LOP,
	ELP,
	CHR,
	PUT,

	COND,
	ECND,
	DRAW,
	EXIT,
	SLP,
	CLS,
	
	CLRS,
	SETA,
	STDA,
	LNFD
} OperationCode;

char opcode_char_list[19] = {
	'+',//ADD,
	'-',//SUB,
	'>',//SHR,
	'<',//SHL,
	'[',//LOP,
	']',//ELP,
	',',//CHR,
	'.',//PUT,

	'{',//COND,
	'}',//ECND,
	'#',//DRAW,
	'$',//EXIT,
	'%',//SLP,
	'*',//CLS,
	
	' ',//CLRS,
	'@',//SETA,
	' ',//STDA,
	'/'//LNFD
};

typedef struct {
	OperationCode seq_tokens[12];
	OperationCode op;
	int arr_size;
} OpCodeSequence;

OpCodeSequence sequence_list[4] = {
	{{LOP, ELP}, EXIT, 2},
	{{LOP, SUB, ELP}, CLRS, 3},
	{{LOP, ADD, ELP}, CLRS, 3},
	{{SETA, SETA}, STDA, 2},
};

typedef struct byte_t {
	int repeats;
	OperationCode opcode;
} Operation;

typedef struct {
	Operation* operations;
	int operations_size, operations_max_size;
} Interpreter;