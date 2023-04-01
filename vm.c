// Author: Priyansh Patel
// Assignment 4: PL/0 Compiler

#include "driver.h"

int base(int stack[], int level, int BP)
{
	int base = BP;
	int counter = level;

	while (counter > 0)
	{
		base = stack[base];
		counter--;
	}
	return base;
}

void execute(instruction **code, int flag)
{
	// intialize values
	int sp = -1;
	int bp = 0;
	int pc = 0;
	instruction *ir = NULL;
	int stack[MAX_STACK_HEIGHT] = {0};
	int input_line_number = 0;
	char name[4];
	int halt = 0;
	int activation_record[MAX_STACK_HEIGHT] = {0};
	int ar_counter = 0;
	int num_ar;
	int counter = 0;

	// dynamically allocate memory for the instruction register
	ir = calloc(1, sizeof(instruction));

	int opcode[MAX_CODE_LENGTH];
	int l[MAX_CODE_LENGTH];
	int m[MAX_CODE_LENGTH];

	// print intial values with apporiate flag value
	if (flag == 1)
	{
		printf("                PC   BP   SP   stack\n");
		printf("Initial values:%2d   %2d    %2d\n", pc, bp, sp);
	}

	for (int i = 0; i < MAX_CODE_LENGTH; i++)
	{
		if (code[i]->opcode == INT_MAX)
		{
			break;
		}
		opcode[i] = code[i]->opcode;
		l[i] = code[i]->l;
		m[i] = code[i]->m;
		counter++;
	}

	// fetch - execute cycles
	for (int i = 0; i < MAX_CODE_LENGTH; i++)
	{
		// updating input_line_number
		input_line_number = pc;

		// break from the loop if halt is true
		if (halt == 1)
		{
			break;
		}

		// fetch cycle
		ir->opcode = opcode[pc];
		ir->l = l[pc];
		ir->m = m[pc];

		// increment program counter
		pc = pc + 1;

		// execute cycle
		switch (ir->opcode)
		{
			// LIT
			case 1:
			sp = sp + 1;
			stack[sp] = ir->m;
			strcpy(name , "LIT");
			break;

			// OPR
			case 2:
			switch (ir->m) {
				// RTN
				case 0:
				stack[bp - 1] = stack[sp];
				sp = bp - 1;
				bp = stack[sp + 2];
				pc = stack[sp + 3];

				// return from AR
				activation_record[ar_counter] = 0;
				ar_counter--;
				strcpy(name , "OPR");
				break;

				// NEG
				case 1:
				stack[sp] = -1 * stack[sp];
				strcpy(name , "OPR");
				break;

				// ADD
				case 2:
				sp = sp - 1;
				stack[sp] = stack[sp] + stack[sp + 1];
				strcpy(name , "OPR");
				break;

				// SUB
				case 3:
				sp = sp - 1;
				stack[sp] = stack[sp] - stack[sp + 1];
				strcpy(name , "OPR");
				break;

				// MUL
				case 4:
				sp = sp - 1;
				stack[sp] = stack[sp] * stack[sp + 1];
				strcpy(name , "OPR");
				break;

				// DIV
				case 5:
				sp = sp - 1;
				stack[sp] = stack[sp] / stack[sp + 1];
				strcpy(name , "OPR");
				break;

				// ODD
				case 6:
				stack[sp] = stack[sp] % 2;
				strcpy(name , "OPR");
				break;

				// MOD
				case 7:
				sp = sp - 1;
				stack[sp] = stack[sp] % stack[sp + 1];
				strcpy(name , "OPR");
				break;

				// EQL
				case 8:
				sp = sp - 1;
				stack[sp] = (stack[sp] == stack[sp + 1]);
				strcpy(name , "OPR");
				break;

				// NEQ
				case 9:
				sp = sp - 1;
				stack[sp] = (stack[sp] != stack[sp + 1]);
				strcpy(name , "OPR");
				break;

				// LSS
				case 10:
				sp = sp - 1;
				stack[sp] = (stack[sp] < stack[sp + 1]);
				strcpy(name , "OPR");
				break;

				// LEQ
				case 11:
				sp = sp - 1;
				stack[sp] = (stack[sp] <= stack[sp + 1]);
				strcpy(name , "OPR");
				break;

				// GTR
				case 12:
				sp = sp - 1;
				stack[sp] = (stack[sp] > stack[sp + 1]);
				strcpy(name , "OPR");
				break;

				// GEQ
				case 13:
				sp = sp - 1;
				stack[sp] = (stack[sp] >= stack[sp + 1]);
				strcpy(name , "OPR");
				break;
			}
			break;

			// LOD
			case 3:
			sp = sp + 1;
			stack[sp] = stack[base(stack, ir->l, bp) + ir->m];
			strcpy(name , "LOD");
			break;

			// STO
			case 4:
			stack[base(stack, ir->l, bp) + ir->m] = stack[sp];
			sp = sp - 1;
			strcpy(name , "STO");
			break;

			// CAL
			case 5:
			stack[sp + 1] = base(stack, ir->l, bp); //	static link
			stack[sp + 2] = bp;											//	dynamic link
			stack[sp + 3] = pc;											//	return address
			stack[sp + 4] = stack[sp];							//	parameter
			bp = sp + 1;
			pc = ir->m;

			activation_record[ar_counter] = bp;			// mark bp for activation record
			ar_counter++;
			strcpy(name , "CAL");
			break;

			// INC
			case 6:
			sp = sp + ir->m;
			strcpy(name , "INC");
			break;

			// JMP
			case 7:
			pc = ir->m;
			strcpy(name , "JMP");
			break;

			// JPC
			case 8:
			if (stack[sp] == 0)
			{
				pc = ir->m;
			}
			sp = sp - 1;
			strcpy(name , "JPC");
			break;

			// SYS
			case 9:
			strcpy(name , "SYS");
			switch (ir->m)
			{
				// write instruction
				case 1:
				if (flag == 1)
				{
					printf("Top of Stack Value: %d\n", stack[sp]);
				}
				sp = sp - 1;
				break;

				// read instruction
				case 2:
				sp = sp + 1;
				if (flag == 1)
				{
					printf("Please Enter an Integer: ");
					scanf("%d", &stack[sp]);
					printf("\n");
				}
				break;

				// halt program
				case 3:
				halt = 1;
				break;
			}
			break;

		}

		// printing each line
		if (flag == 1)
		{
			// print header
			printf("%2d %s  %2d %2d", input_line_number, name, ir->l, ir->m);
			printf("   %2d  %2d    %2d  ", pc, bp, sp);

			// printing activation record and the stack
			for (int j = 0; j <= sp; j++)
			{
				if ((ar_counter > num_ar) && (j == activation_record[num_ar]))
				{
					printf("| ");
					num_ar++;
				}

				// print stack
				printf("%d ", stack[j]);
			}

			num_ar = 0;
			printf("\n");
		}
	}
}
