// Author: Priyansh Patel
// Assignment 4: PL/0 Compiler

#include "driver.h"

lexeme **lex_analyze(char* file, int flag)
{
  // structs
  lexeme **list = malloc(MAX_TOKEN_LIST * sizeof(*list));    // array of lists
  lexeme tokens;                                             // lexeme struct

  // variables
  int future = 0;           // check to determine if the next char has been read
  int digit_counter = 0;    // iterate to copy string of numbers
  int alpha_counter = 0;    // iterate to copy string of alphanumeric chars
  int error_check = 0;      // checks any error occured
  char character;           // holds each char from fgetc()
  char token_list[BUFFER];  // char array to output token list
  int counter = 0;          // counter to keep track of array of list(lexeme)

  FILE* f = NULL;

  // allocate array of structs (lexeme)
  for (int i = 0; i < MAX_TOKEN_LIST; i++)
  {
    list[i] = malloc(sizeof(**list));
  }

  // open file
  f = fopen(file, "r");

  // print header for lexeme table if flag is 1
  if (flag == 1)
  {
    printf("Lexeme Table: \n");
    printf("lexeme          token type\n");
  }

  token_list[0] = '\0';      // set token list array

  // first file character
  character = fgetc(f);

  // terminate if end of file is reached
  while (character != EOF)
  {
    // ignore control characters
    if (iscntrl(character) || isspace(character))
    {
      character = fgetc(f);
      continue;
    }

    // symbols
    if (!isalnum(character))
    {
      // modsym
      if (character == '%')
      {
        tokens.number = 1;
        list[counter]->type = modsym;
        counter++;

        strcat(token_list, "1 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // plussym
      else if (character == '+')
      {
        tokens.number = 4;
        list[counter]->type = plussym;
        counter++;

        strcat(token_list, "4 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // minussym
      else if (character == '-')
      {
        tokens.number = 5;
        list[counter]->type = minussym;
        counter++;

        strcat(token_list, "5 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // multsym
      else if (character == '*')
      {
        tokens.number = 6;
        list[counter]->type = multsym;
        counter++;

        strcat(token_list, "6 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // eqlsym
      else if (character == '=')
      {
        tokens.number = 9;
        list[counter]->type = eqlsym;
        counter++;

        strcat(token_list, "9 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // lparentsym
      else if (character == '(')
      {
        tokens.number = 15;
        list[counter]->type = lparentsym;
        counter++;

        strcat(token_list, "15 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // rparentsym
      else if (character == ')')
      {
        tokens.number = 16;
        list[counter]->type = rparentsym;
        counter++;

        strcat(token_list, "16 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // commasym
      else if (character == ',')
      {
        tokens.number = 17;
        list[counter]->type = commasym;
        counter++;

        strcat(token_list, "17 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // semicolonsym
      else if (character == ';')
      {
        tokens.number = 18;
        list[counter]->type = semicolonsym;
        counter++;

        strcat(token_list, "18 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // periodsym
      else if (character == '.')
      {
        tokens.number = 19;
        list[counter]->type = periodsym;
        counter++;

        strcat(token_list, "19 ");
        if (flag == 1)
        {
          printf("         %c\t  %d\n",character, tokens.number);
        }
      }
      // slashsym (possible start of comments)
      else if (character == '/')
      {
        character = fgetc(f);
        future = 1;

        // comments
        if (character == '*')
        {
          while (character != '|')
          {
            character = fgetc(f);

            if (character == '*')
            {
              character = fgetc(f);
              if (character == '/')
              {
                character = fgetc(f);
                future = 1;
                break;
              }
            }
          }
        }

        // slashsym
        else
        {
          strcpy(tokens.name, "/");
          tokens.number = 7;
          list[counter]->type = slashsym;
          counter++;

          strcat(token_list, "7 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
      }
      // lessym (could also be neqsym || leqsym)
      else if (character == '<')
      {
        character = fgetc(f);
        future = 0;

        // neqsym
        if (character == '>')
        {
          strcpy(tokens.name, "<>");
          tokens.number = 10;
          list[counter]->type = neqsym;
          counter++;

          strcat(token_list, "10 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        // leqsym
        else if (character == '=')
        {
          strcpy(tokens.name, "<=");
          tokens.number = 12;
          list[counter]->type = leqsym;
          counter++;

          strcat(token_list, "12 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        // lessym
        else
        {
          strcpy(tokens.name, "<");
          tokens.number = 11;
          list[counter]->type = lessym;
          counter++;

          strcat(token_list, "11 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
      }
      // gtrsym (could also be geqsym)
      else if (character == '>')
      {
        character = fgetc(f);
        future = 0;

        // geqsym
        if (character == '=')
        {
          strcpy(tokens.name, ">=");
          tokens.number = 14;
          list[counter]->type = geqsym;
          counter++;

          strcat(token_list, "14 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        // gtrsym
        else
        {
          strcpy(tokens.name, ">");
          tokens.number = 13;
          list[counter]->type = gtrsym;
          counter++;

          strcat(token_list, "13 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
      }
      // becomessym (if next char is =)
      else if (character == ':')
      {
        character = fgetc(f);
        future = 1;     // check to hold next char

        if (character == '=')
        {
          strcpy(tokens.name, ":=");
          tokens.number = 20;
          list[counter]->type = becomessym;
          counter++;

          strcat(token_list, "20 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else
        {
          printf("Error : Invalid Symbol\n");
          exit(1);
        }

        character = fgetc(f);
        future = 1;
      }
      else
      {
        printf("Error : Invalid Symbol\n");
        exit(1);
      }

      // read next char
      if (future == 0)
      {
        character = fgetc(f);
      }
      // next char already read
      else
      {
        future = 0;
      }

      continue;
    }

    // alphabet
    if (isalpha(character))
    {
      error_check = 0;      // no error yet

      // read until whitespace
      while(isalnum(character))
      {
        tokens.name[alpha_counter] = character;
        tokens.name[alpha_counter + 1] = '\0';    // set null terminator
        alpha_counter++;
        character = fgetc(f);
        future = 1;

        // error message (exceed 11 chars)
        if (alpha_counter >= 11)
        {
          printf("Error : Identifier names cannot exceed 11 characters\n");
          error_check = 1;
          exit(1);

          tokens.name[0] = '\0';

          // read until whitespace
          while(isalnum(character))
          {
            character = fgetc(f);
          }

          break;
        }
      }

      // no error occured (check is its a reserve word or not)
      if (error_check == 0)
      {
        // reserver words
        if (strcmp(tokens.name, "odd") == 0)
        {
          tokens.number = 8;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = oddsym;
          counter++;

          strcat(token_list, "8 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "begin") == 0)
        {
          tokens.number = 21;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = beginsym;
          counter++;

          strcat(token_list, "21 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "end") == 0)
        {
          tokens.number = 22;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = endsym;
          counter++;

          strcat(token_list, "22 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "if") == 0)
        {
          tokens.number = 23;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = ifsym;
          counter++;

          strcat(token_list, "23 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "then") == 0)
        {
          tokens.number = 24;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = thensym;
          counter++;

          strcat(token_list, "24 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "while") == 0)
        {
          tokens.number = 25;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = whilesym;
          counter++;

          strcat(token_list, "25 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "do") == 0)
        {
          tokens.number = 26;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = dosym;
          counter++;

          strcat(token_list, "26 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "call") == 0)
        {
          tokens.number = 27;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = callsym;
          counter++;

          strcat(token_list, "27 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "const") == 0)
        {
          tokens.number = 28;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = constsym;
          counter++;

          strcat(token_list, "28 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "var") == 0)
        {
          tokens.number = 29;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = varsym;
          counter++;

          strcat(token_list, "29 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "procedure") == 0)
        {
          tokens.number = 30;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = procsym;
          counter++;

          strcat(token_list, "30 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "write") == 0)
        {
          tokens.number = 31;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = writesym;
          counter++;

          strcat(token_list, "31 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "read") == 0)
        {
          tokens.number = 32;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = readsym;
          counter++;

          strcat(token_list, "32 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "else") == 0)
        {
          tokens.number = 33;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = elsesym;
          counter++;

          strcat(token_list, "33 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        else if (strcmp(tokens.name, "return") == 0)
        {
          tokens.number = 34;
          strcpy(list[counter]->name, tokens.name);
          list[counter]->type = returnsym;
          counter++;

          strcat(token_list, "34 ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
        // non reserve words (variable names)
        else
        {
          tokens.number = 2;
          strcat(list[counter]->name, tokens.name);
          list[counter]->type = identsym;
          counter++;

          strcat(token_list, "2 ");
          strcat(token_list, tokens.name);
          strcat(token_list, " ");
          if (flag == 1)
          {
            printf("         %s\t  %d\n",tokens.name, tokens.number);
          }
        }
      }

      // read next char
      if (future == 0)
      {
        character = fgetc(f);
      }
      // next char already read
      else
      {
        future = 0;
      }

      alpha_counter = 0;      // reset counter for next check
      continue;
    }

    // digits
    if(isdigit(character))
    {
      error_check = 0;       // no error yet

      // read until whitespace or letter
      while(isdigit(character))
      {
        tokens.name[digit_counter] = character;
        tokens.name[digit_counter + 1] = '\0';
        digit_counter++;
        character = fgetc(f);
        future = 1;

        // error message (exceed 5 numbers)
        if (digit_counter >= 5)
        {
          printf("Error : Numbers cannot exceed 5 digits\n");
          error_check = 1;
          exit(1);

          tokens.name[0] = '\0';

          while(isalnum(character))
          {
            character = fgetc(f);
          }

          break;
        }

        // error message (letter dectected)
        if (isalpha(character))
        {
          printf("Error : Identifiers cannot begin with a digit\n");
          error_check = 1;
          exit(1);

          tokens.name[0] = '\0';

          // read until whitespace
          while(isalnum(character))
          {
            character = fgetc(f);
          }

          break;
        }
      }

      // tokenize
      if (error_check == 0)
      {
        tokens.number = 3;
        list[counter]->number = atoi(tokens.name);
        list[counter]->type = numbersym;
        counter++;

        strcat(token_list, "3 ");
        strcat(token_list, tokens.name);
        strcat(token_list, " ");
        if (flag == 1)
        {
          printf("         %s\t  %d\n",tokens.name, tokens.number);
        }
        error_check = 0;
      }


      // read next char
      if (future == 0)
      {
        character = fgetc(f);
      }
      // next char already read
      else
      {
        future = 0;
      }

      digit_counter = 0;
      continue;
    }
  }

  // close file
  fclose(f);

  // flag to stop reading for parser.c
  list[counter]->type = INT_MAX;

  // Print token list
  if (flag == 1)
  {
    printf("\nToken List:\n");
    printf("%s\n\n\n", token_list);
  }

  // array of lexeme
  return list;
}
