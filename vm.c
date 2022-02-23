/*
  You can use these two print statements for the errors:
    printf("Virtual Machine Error: Stack Overflow Error\n");
    printf("Virtual Machine Error: Out of Bounds Access Error\n");
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "vm.h"
#define REG_FILE_SIZE 10
#define MAX_STACK_LENGTH 100

void print_execution(int line, char *opname, instruction IR, int PC, int BP, int SP, int *stack, int *RF)
{
  int i;
  // print out instruction and registers
  printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t\t", line, opname, IR.r, IR.l, IR.m, PC, SP, BP);

  // print register file
  for (i = 0; i < REG_FILE_SIZE; i++)
    printf("%d ", RF[i]);
  printf("\n");

  // print stack
  printf("stack:\t");
  for (i = MAX_STACK_LENGTH - 1; i >= SP; i--)
    printf("%d ", stack[i]);
  printf("\n");
}

int base(int L, int BP, int *stack)
{
  int ctr = L;
  int rtn = BP;
  while (ctr > 0)
  {
    rtn = stack[rtn];
    ctr--;
  }
  return rtn;
}

void execute_program(instruction *code, int printFlag)
{
  // Initialize base pointer, stack pointer, and program counter.
  int BP = MAX_STACK_LENGTH - 1;
  int SP = BP + 1;
  int PC = 0;

  // Enter fetch-execute cycle.
  int halt = 0;
  while (!halt)
  {
    // Fetch an instruction and place in IR register
    // Increment program counter
    instruction IR;
    IR = code[PC++];

    // Execute instruction based off instruction opcode
    switch (IR.opcode)
    {
    default:
      printf("bad opcode");
      break;

    case LIT:
      // RF[IR.R] = IR.M
      break;

    case RET:
      // Return from current procedure (X) to the last procedure (Y).
      // SP = the index of the end of Y’s AR (BP + 1)
      // BP = dynamic link value from X’s AR
      // PC = return address value from X’s AR
      break;

    case LOD:
      // Load value to register IR.R from the stack location at
      // offset RF[IR.M] from L lexicographical levels up
      // RF[IR.R] = stack[base(L) - RF[IR.M]]
      // Before performing the load, check for Out of Bounds
      // Access Error by checking if base(L) - M is less than
      // zero or greater than or equal to MAX_STACK_LENGTH
      break;

    case STO:
      // Store value from register IR.R to the stack location at
      // offset RF[IR.M] from L lexicographical levels down
      // stack[base(L) - RF[IR.M]] = RF[IR.R]
      // Before performing the load, check for Out of Bounds
      // Access Error by checking if base(L) - M is less than
      // zero or greater than or equal to MAX_STACK_LENGTH
      break;

    case CAL:
      // Call procedure at code index M. This will generate a
      // new Activation Record. There are three values in the
      // AR:
      //  1st - static link = base(L)
      //  2nd - dynamic link = BP
      //  3rd - return address = PC
      // After creating the activation record,
      //  BP = the index of the first entry of the new AR
      //  PC = IR.M
      break;

    case INC:
      // Decrement SP by M, check for Stack Overflow Error
      // which can occur if SP < 0 after the decrement
      break;

    case JMP:
      // Jump to instruction M
      break;

    case JPC:
      // Jump to instruction M if register R is 0
      break;

    case WRT:
      // Print register R
      break;

    case RED:
      // Register R equals scanf()
      break;

    case HAL:
      // End of program (Set Halt flag to true)
      halt = 1;
      break;

    case NEG:
      // Negate the register R
      break;

    case ADD:
      // Add the registers L and M and store the result
      // in register R
      break;

    case SUB:
      // Subtract register M from register L and store the result
      // in register R
      break;

    case MUL:
      // Multiply registers L and M and store the result
      // in register R
      break;

    case DIV:
      // Divide register L by register M and store the result
      // in register R
      break;

    case MOD:
      // Set register R equal to register L modulo register M
      break;

    case EQL:
      // If register L equals register M, set register R to 1.
      // Otherwise set register R to 0
      break;

    case NEQ:
      // If register L does not equal register M, set register R to
      // 1. Otherwise set register R to 0
      break;

    case LSS:
      // If register L is less than register M, set register R to 1.
      // Otherwise set register R to 0
      break;

    case LEQ:
      // If register L is less than or equal to register M, set register R to 1.
      // Otherwise set register R to 0
      break;

    case GTR:
      // If register L is greater than register M, set register R to  1.
      // Otherwise set register R to 0
      break;

    case GEQ:
      // If register L is greater than or equal to register M, set register R to 1.
      // Otherwise set register R to 0
      break;
    }
  }
  // keep this
  if (printFlag)
  {
    printf("\t\t\t\t\tPC\tSP\tBP\n");
    printf("Initial values:\t\t\t\t%d\t%d\t%d\n", PC, SP, BP);
  }
}