// Author: Priyansh Patel
// Assignment 4: PL/0 Compiler

#include "driver.h"

// structs
symbol table[MAX_SYMBOL_TABLE_SIZE];
instruction **code;
lexeme **list;

// Global Variables
int token;                // current token
char token_name[12];      // current token name
int token_number;         // current token number
int tokenIdx = 0;         // counter for token
int codeIdx = 0;          // counter for code (struct)
int tableIdx = 0;         // counter for table()
int procIdx = 0;          // Used in Procedure_Declaration()
int jpcIdx;               // Used in Statement()
int jmpIdx;               // used in Statement()
int loopIdx;              // used in Statement()
int numVars;              // number of vars
char save_ident[12];      // save identifier name
int procedurecount = 0;   // keeps count of procedure

// complier function
instruction **parse(lexeme **lexeme_list, int flag)
{
  list = lexeme_list;         // points to global struct

  instruction **instruct = malloc(MAX_CODE * sizeof(*code));

  // allocate array of structs (instruction)
  for (int i = 0; i < MAX_CODE; i++)
  {
    instruct[i] = malloc(sizeof(**code));
  }

  code = instruct;            // points to global struct

  Get_Token();

  Program();                  // runs the parser

  instruct = code;            // instruct gets the correct pointer

  instruct[codeIdx]->opcode = INT_MAX;    // flag opcode to stop printing

  if (flag == 1)
  {
    printf("Generated Assembly: \n");
    printf("Line    OP    L    M \n");

    for (int i = 0; i < MAX_CODE; i++)
    {
      if (code[i]->opcode == INT_MAX)
      {
        break;
      }
      printf("  %2d    %s    %2d    %2d\n", i, code[i]->op, code[i]->l, code[i]->m);
    }

    printf("\n\n");
  }

  return instruct;
}

// creates a code entry in code struct array
void Emit(int opcode, char op[], int l, int m)
{
  code[codeIdx]->opcode = opcode;
  strcpy(code[codeIdx]->op, op);
  code[codeIdx]->l = l;
  code[codeIdx]->m = m;
  codeIdx++;
  return;
}

// creates a symbol entry in table struct array
void Enter(int type, char name[], int number, int level, int addr, int mark, int param)
{
  table[tableIdx].kind = type;
  strcpy(table[tableIdx].name, name);
  table[tableIdx].val = number;
  table[tableIdx].level = level;
  table[tableIdx].addr = addr;
  table[tableIdx].mark = mark;
  table[tableIdx].param = param;
  tableIdx++;
  return;
}

// takes the next token in the list
void Get_Token()
{
  token = list[tokenIdx]->type;
  token_number = list[tokenIdx]->number;
  strcpy(token_name, list[tokenIdx]->name);
  tokenIdx++;
  return;
}

// called when Error is detected
void Error(char* string)
{
  printf("Error: %s\n", string);
  exit(1);
}

// find prodecure using given procedure index
int Find_Procedure(int procedureIndex)
{
  // linear search through Symbol table
  for (int i = 0; i < tableIdx; i++)
  {
    if ((table[i].kind == 3) && (table[i].val == procedureIndex))
    {
      return i;
    }
  }

  return -1;
}

// marks symbols out of scope
void Mark(int count)
{
  for (int i = tableIdx; i > 0; i--)
  {
    if (table[i].mark == 0)
    {
      table[i].mark = 1;
      count--;
    }
    else
    {
      continue;
    }
  }

  return;
}

// search through the Symbol table for the name
int Symbol_Table_Check(char* name, int level)
{
  // linear search through Symbol table
  for (int i = 0; i < tableIdx; i++)
  {
    if ((strcmp(table[i].name, name) == 0) && (table[i].level == level) && table[i].mark == 0)
    {
      return i;
    }
  }

  return -1;
}

// search through the Symbol table for the name
int Symbol_Table_Search(char* name, int level, int kind)
{
  int array[100];
  int count = 0;
  int closest = 0;
  int temp = 0;

  // linear search through Symbol table
  for (int i = 0; i < tableIdx; i++)
  {
    if (strcmp(name, table[i].name) == 0 && table[i].kind == kind)
    {
      if (table[i].level <= level)
      {
        array[count] = i;
        count++;
      }
    }
  }

  for (int j = 0; j < count; j++)
  {
    temp = table[array[j]].level - level;

    if (temp <= (table[array[j]].level - level) )
    {
      temp = array[j];
      closest = 1;
    }
  }

  if (closest == 1)
  {
    return temp;
  }

  return -1;
}

// Start Parsing
void Program()
{
  int numProc = 1;

  Emit(7, "JMP", 0, numProc);

  for(int i = 0; i < MAX_TOKEN_LIST; i++)
  {
    if (list[i]->type == procsym)
    {
      numProc++;
      Emit(7, "JMP", 0, numProc);
    }
  }

  Enter(3, "main", 0, 0, 0, 0, 0);

  procedurecount++;

  Block(0, 0, 0);

  if (token != periodsym)
  {
    Error("program must end with period");
  }

  for (int i = 0; i < numProc; i++)
  {
    code[i]->m = table[Find_Procedure(i)].addr;
  }

  for (int j = 0; j < codeIdx; j++)
  {
    if (code[j]->opcode == 5)
    {
      code[j]->m = table[Find_Procedure(code[j]->m)].addr;
    }
  }

  Emit(9, "SYS", 0, 3);               // halt
}

void Block(int lexlevel, int param, int procedureIndex)
{
  int c ,v ,p;

  c = Const_Declaration(lexlevel);
  v = Var_Declaration(lexlevel, param);
  p = Procedure_Declaration(lexlevel);

  table[procedureIndex].addr = codeIdx;

  Emit(6, "INC", 0, v + 4);

  Statement(lexlevel);
  Mark(c + v + p);
}

int Const_Declaration(int lexlevel)
{
  int numConst = 0;

  if (token == constsym)
  {
    do
    {
      numConst++;
      Get_Token();

      if (token != identsym)
      {
        Error("const, var, procedure, call, and read keywords must be followed by identifier");
      }

      if (Symbol_Table_Check(token_name, lexlevel) != -1)
      {
        Error("competing symbol declarations at the same level");
      }

      // save idetifier name
      strcpy(save_ident, token_name);

      Get_Token();

      if (token != eqlsym)
      {
        Error("constants must be assigned with =");
      }

      Get_Token();

      if (token != numbersym)
      {
        Error("constants must be assigned an integer value");
      }

      // create a symbol table entry
      Enter(1, save_ident, token_number, lexlevel, 0, 0, 0);

      Get_Token();

    } while(token == commasym);

    if (token != semicolonsym)
    {
      Error("symbol declarations must be followed by a semicolon");
    }

    Get_Token();
  }
  return numConst;
}

int Var_Declaration(int lexlevel, int param)
{
  int numVars;              //  number of variables

  if (param == 1)
  {
    numVars = 1;
  }
  else
  {
    numVars = 0;
  }

  if (token == varsym)
  {
    do
    {
      numVars++;

      Get_Token();

      if(token != identsym)
      {
        Error("const, var, procedure, call, and read keywords must be followed by identifier");
      }

      if (Symbol_Table_Check(token_name, lexlevel) != -1)
      {
        Error("competing symbol declarations at the same level");
      }

      Enter(2, token_name, 0, lexlevel, numVars + 3, 0, 0);

      Get_Token();

    } while(token == commasym);

    if (token != semicolonsym)
    {
      Error("symbol declarations must be followed by a semicolon");
    }

    Get_Token();
  }

  return numVars;
}

int Procedure_Declaration(int lexlevel)
{
  int numProc = 0;

  if (token == procsym)
  {
    do {
      numProc++;
      Get_Token();

      if(token != identsym)
      {
        Error("const, var, procedure, call, and read keywords must be followed by identifier");
      }

      if (Symbol_Table_Check(token_name, lexlevel) != -1)
      {
        Error("competing symbol declarations at the same level");
      }

      procIdx = tableIdx;

      Enter(3, token_name, procedurecount, lexlevel, 0, 0, 0);

      procedurecount++;
      Get_Token();

      if (token == lparentsym)
      {
        Get_Token();

        if(token != identsym)
        {
          Error("const, var, procedure, call, and read keywords must be followed by identifier");
        }

        Enter(2, token_name, 0, lexlevel + 1, 3, 0, 0);

        table[procIdx].param = 1;

        Get_Token();

        if (token != rparentsym)
        {
          Error("right parenthesis must follow left parenthesis");
        }

        Get_Token();

        if (token != semicolonsym)
        {
          Error("symbol declarations must be followed by a semicolon");
        }

        Get_Token();

        Block(lexlevel + 1, 1, procIdx);
      }
      else
      {
        if (token != semicolonsym)
        {
          Error("symbol declarations must be followed by a semicolon");
        }

        Get_Token();
        Block(lexlevel + 1, 0, procIdx);
      }

      if ((code[codeIdx - 1]->opcode != 2) && (code[codeIdx - 1]->m != 0))
      {
        Emit(1, "LIT", 0, 0);
        Emit(2, "OPR", 0, 0);
      }

      if (token != semicolonsym)
      {
        Error("symbol declarations must be followed by a semicolon");
      }

      Get_Token();

    } while(token == procsym);
  }

  return numProc;
}

void Statement(int lexlevel)
{
  int symIdx = 0;

  if (token == identsym)
  {
    symIdx = Symbol_Table_Search(token_name, lexlevel, 2);
    if (symIdx == -1)
    {
      Error("undeclared variable or constant in equation");
    }

    Get_Token();

    if (token != becomessym)
    {
      Error("assignment statements must use :=");
    }

    Get_Token();
    Expression(lexlevel);
    Emit(4 , "STO", lexlevel - table[symIdx].level, table[symIdx].addr);
    return;
  }

  if (token == callsym)
  {
    Get_Token();

    if (token != identsym)
    {
      Error("const, var, procedure, call, and read keywords must be followed by identifier");
    }

    symIdx = Symbol_Table_Search(token_name, lexlevel, 3);

    if (symIdx == -1)
    {
      Error("undeclared procedure for call");
    }

    Get_Token();

    if (token == lparentsym)
    {
      Get_Token();

      if (table[symIdx].param != 1)
      {
        Error("parameters must be declared");
      }

      Expression(lexlevel);

      if (token != rparentsym)
      {
        Error("right parenthesis must follow left parenthesis");
      }

      Get_Token();
    }
    else
    {
      Emit(1, "LIT", 0, 0);
    }

    Emit(5, "CAL", lexlevel - table[symIdx].level, table[symIdx].val);

    return;
  }

  if (token == returnsym)
  {
    if (lexlevel == 0)
    {
      Error("cannot return from main");
    }

    Get_Token();

    if (token == lparentsym)
    {
      Get_Token();
      Expression(lexlevel);
      Emit(2, "OPR", 0, 0);

      if (token != rparentsym)
      {
        Error("right parenthesis must follow left parenthesis");
      }

      Get_Token();
    }
    else
    {
      Emit(1, "LIT", 0, 0);
      Emit(2, "OPR", 0, 0);
    }

    return;
  }

  if (token == beginsym)
  {
    do
    {
      Get_Token();
      Statement(lexlevel);
    } while(token == semicolonsym);

    if (token != endsym)
    {
      Error("begin must be followed by end");
    }

    Get_Token();

    return;
  }

  if (token == ifsym)
  {
    Get_Token();

    Condition(lexlevel);

    jpcIdx = codeIdx;

    Emit(8, "JPC", 0, 0);

    if (token != thensym)
    {
      Error("if must be followed by then");
    }

    Get_Token();

    Statement(lexlevel);

    if (token == elsesym)
    {
      Get_Token();
      jmpIdx = codeIdx;
      Emit(7, "JMP", 0, 0);
      code[jpcIdx]->m = codeIdx;
      Statement(lexlevel);
      code[jmpIdx]->m = codeIdx;
    }
    else
    {
      code[jpcIdx]->m = codeIdx;
    }

    return;
  }

  if (token == whilesym)
  {
    Get_Token();

    loopIdx = codeIdx;

    Condition(lexlevel);

    if (token != dosym)
    {
      Error("while must be followed by do");
    }

    Get_Token();

    jpcIdx = codeIdx;

    Emit(8, "JPC", 0, 0);

    Statement(lexlevel);

    Emit(7, "JMP", 0, 0);

    code[jpcIdx]->m = codeIdx;

    return;
  }

  if (token == readsym)
  {
    Get_Token();

    if (token != identsym)
    {
      Error("const, var, procedure, call, and read keywords must be followed by identifier");
    }

    symIdx = Symbol_Table_Search(token_name, lexlevel, 2);
    if (symIdx == -1)
    {
      Error("undeclared variable or constant in equation");
    }

    Get_Token();
    Emit(9, "SYS", 0, 2);
    Emit(4, "STO", lexlevel - table[symIdx].level, table[symIdx].addr);

    return;
  }

  if (token == writesym)
  {
    Get_Token();
    Expression(lexlevel);
    Emit(9, "SYS", 0, 1);
    return;
  }
}

void Condition(int lexlevel)
{
  if (token == oddsym)
  {
    Get_Token();

    Expression(lexlevel);

    Emit(2, "OPR", 0, 6);
  }
  else
  {
    Expression(lexlevel);

    if (token == eqlsym)
    {
      Get_Token();
      Expression(lexlevel);
      Emit(2, "OPR", 0, 8);
    }
    else if (token == neqsym)
    {
      Get_Token();
      Expression(lexlevel);
      Emit(2, "OPR", 0, 9);
    }
    else if (token == lessym)
    {
      Get_Token();
      Expression(lexlevel);
      Emit(2, "OPR", 0, 10);
    }
    else if (token == leqsym)
    {
      Get_Token();
      Expression(lexlevel);
      Emit(2, "OPR", 0, 11);
    }
    else if (token == gtrsym)
    {
      Get_Token();
      Expression(lexlevel);
      Emit(2, "OPR", 0, 12);
    }
    else if (token == geqsym)
    {
      Get_Token();
      Expression(lexlevel);
      Emit(2, "OPR", 0, 13);
    }
    else
    {
      Error("condition must contain comparison operator");
    }
  }
}

void Expression(int lexlevel)
{
  if (token == minussym)
  {
    Get_Token();
    Term(lexlevel);
    Emit(2, "OPR", 0, 1);

    while (token == plussym || token == minussym)
    {
      if (token == plussym)
      {
        Get_Token();
        Term(lexlevel);
        Emit(2, "OPR", 0, 2);
      }
      else
      {
        Get_Token();
        Term(lexlevel);
        Emit(2, "OPR", 0, 3);
      }
    }
  }
  else
  {
    if (token == plussym)
    {
      Get_Token();
    }

    Term(lexlevel);

    while (token == plussym || token == minussym)
    {
      if (token == plussym)
      {
        Get_Token();
        Term(lexlevel);
        Emit(2, "OPR", 0, 2);
      }
      else
      {
        Get_Token();
        Term(lexlevel);
        Emit(2, "OPR", 0, 3);
      }
    }
  }

  return;
}

void Term(int lexlevel)
{
  Factor(lexlevel);

  while (token == multsym || token == slashsym || token == modsym)
  {
    if (token == multsym)
    {
      Get_Token();
      Factor(lexlevel);
      Emit(2, "OPR", 0, 4);
    }
    else if (token == slashsym)
    {
      Get_Token();
      Factor(lexlevel);
      Emit(2, "OPR", 0, 5);
    }
    else
    {
      Get_Token();
      Factor(lexlevel);
      Emit(2, "OPR", 0, 7);
    }
  }

  return;
}

void Factor(int lexlevel)
{
  int symIdxV;
  int symIdxC;

  if (token == identsym)
  {
    symIdxV  = Symbol_Table_Search(token_name, lexlevel, 2);
    symIdxC  = Symbol_Table_Search(token_name, lexlevel, 1);
    if (symIdxC == -1 && symIdxV == -1)
    {
      Error("undeclared variable or constant in equation");
    }
    else if (symIdxC == -1 || (symIdxV != -1 && table[symIdxV].level > table[symIdxC].level))
    {
      Emit(3, "LOD", lexlevel - table[symIdxV].level, table[symIdxV].addr);
    }
    else
    {
      Emit(1, "LIT", 0, table[symIdxC].val);
    }
    Get_Token();
  }
  else if (token == numbersym)
  {
    Emit(1, "LIT", 0, token_number);
    Get_Token();
  }
  else if (token == lparentsym)
  {
    Get_Token();
    Expression(lexlevel);

    if (token != rparentsym)
    {
      Error("right parenthesis must follow left parenthesis");
    }

    Get_Token();
  }
  else if (token == callsym)
  {
    Statement(lexlevel);
  }
  else
  {
    Error("arithmetic equations must contain operands, parentheses, numbers, or symbols");
  }

  return;
}
