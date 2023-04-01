// Author: Priyansh Patel
// Assignment 4: PL/0 Compiler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// enum token_types
typedef enum
{
	modsym = 1, identsym, numbersym, plussym, minussym,
	multsym, slashsym, oddsym, eqlsym, neqsym,
	lessym, leqsym, gtrsym, geqsym, lparentsym,
	rparentsym, commasym, semicolonsym, periodsym, becomessym,
	beginsym, endsym, ifsym, thensym, whilesym,
	dosym, callsym,	constsym, varsym, procsym,
	writesym, readsym, elsesym, returnsym
} token_type;

// used by lex.c
#define BUFFER 1024
#define MAX_TOKEN_LIST 500
#define INT_MAX 999999

// used by parser.c
#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_CODE 500

// used by vm.c
#define MAX_STACK_HEIGHT 50
#define MAX_CODE_LENGTH 500

// token struct
typedef struct lexeme
{
	char name[12];       // lexeme
	int number;          // token number
	int type;            // enum type
} lexeme;

// instruction struct
typedef struct instruction
{
	int opcode;
	char op[4];
	int l;
	int m;
} instruction;

// symbol table struct
typedef struct symbol
{
	int kind;          // const = 1, var = 2, proc = 3
	char name[12];     // name
	int val;           // value
	int level;         // L level
	int addr;          // M level
	int mark;					 // to indicate that code has been generated already for a block.
	int param;         // to indicate if the parameter for a procedure has been declared.
} symbol;

// For constants, store kind, name and value.
// For variables, store kind, name, L and M.
// For procedures, store Kind, name, L and M.

// functions for compiler
lexeme **lex_analyze(char* file, int flag);
instruction **parse(lexeme **lexeme_list, int flag);
void execute(instruction **code, int flag);

// helper functions
// used in vm.c
int base(int stack[], int level, int BP);

// used in parser.c
void Emit(int opcode, char op[], int l, int m);
void Enter(int type, char name[], int number, int level, int addr, int mark, int param);
void Get_Token();
void Error(char* string);
int Find_Procedure(int procedureIndex);
void Mark(int count);
int Symbol_Table_Check(char* name, int level);
int Symbol_Table_Search(char* name, int level, int kind);
void Program();
void Block(int lexlevel, int param, int procedureIndex);
int Const_Declaration(int lexlevel);
int Var_Declaration(int lexlevel, int param);
int Procedure_Declaration(int lexlevel);
void Statement(int lexlevel);
void Condition(int lexlevel);
void Expression(int lexlevel);
void Term(int lexlevel);
void Factor(int lexlevel);

/*  Errors
- program must end with period
- const, var, procedure, call, and read keywords must be followed by identifier
- competing symbol declarations at the same level
- constants must be assigned with =
- constants must be assigned an integer value
- symbol declarations must be followed by a semicolon
- undeclared variable or constant in equation
- only variable values may be altered
- assignment statements must use :=
- begin must be followed by end
- if must be followed by then
- while must be followed by do
- condition must contain comparison operator
- right parenthesis must follow left parenthesis
- arithmetic equations must contain operands, parentheses, numbers, or symbols
- undeclared procedure for call
- parameters may only be specified by an identifier
- parameters must be declared
- cannot return from main
*/


/* (use for debugging program)
printf("current symidx: %d\n", symIdx);
printf("Current tableidx: %d\n", tableIdx);
printf("current tokenidx: %d\n", tokenIdx);
printf("name: %s\n", token_name);
printf("token: %d\n", token);
Get_Token();
printf("name: %s\n", token_name);
printf("token: %d\n", token);
printf("lexlevel: %d\n", lexlevel);
printf("++++++++++++++++++++++++++++++++\n");

printf("symIdx: %d\n", symIdx);
for (int i = 0; i < tableIdx; i++)
{
printf("index: %d || kind: %d || name: %s || val: %d || level: %d || addr: %d || mark: %d || param: %d\n", i,
table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark, table[i].param);
}


printf("opcode: %d | op: %s | l: %d | m: %d\n", code[input_line_number]->opcode, code[input_line_number]->op, code[input_line_number]->l, code[input_line_number]->m);
*/
