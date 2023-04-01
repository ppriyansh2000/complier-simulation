Author: Priyansh Patel
Assignment 4: PL/0 Compiler

------instructions-----------

// to compile the program
$ make

// to run the program
./a.out <input.txt> <compiler drivers>


------Compiler Drivers---------
1) -l prints lexmem table and token list (HW2 output)
2) -a prints the generated assembly (HW3 output)
3) -v prints the stack trace (HW1 output)

example
./a.out input_file.txt -l -a -v
