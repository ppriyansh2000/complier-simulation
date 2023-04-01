// Author: Priyansh Patel
// Assignment 4: PL/0 Compiler

#include "driver.h"

int main(int argc, char **argv)
{
	// variables and pointers
	int l = 0;
	int a = 0;
	int v = 0;

	lexeme **list;          // pointer for array of token structs
	instruction **code;     // pointer for array of instruction structs

	// exit if there is no input file included
	if (argc < 2)
	{
		printf("Error : please include the file name");
		return 0;
	}

	// checks the flags for correct output
	if (argc == 5)
	{
		l = 1;
		a = 1;
		v = 1;
	}
	else if (argc == 3 || argc == 4)
	{
		if (argv[2][1] == 'l')
		l = 1;
		else if (argv[2][1] == 'a')
		a = 1;
		else
		v = 1;

		if (argc == 4)
		{
			if (argv[3][1] == 'l')
			l = 1;
			else if (argv[3][1] == 'a')
			a = 1;
			else
			v = 1;
		}
	}

	// compiler functions
	list = lex_analyze(argv[1], l);					// Lexical Analyzer
	code = parse(list, a);									// Parser and Code Generation
	execute(code, v);												// Virtual Machine

	return 0;
}
