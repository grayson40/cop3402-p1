/*
  You can use these two print statements for the errors:
    printf("Virtual Machine Error: Stack Overflow Error\n");
    printf("Virtual Machine Error: Out of Bounds Access Error\n");
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define REG_FILE_SIZE 10
#define MAX_STACK_LENGTH 100

// Map opcodes integer value to opname
#define LIT 1
#define RET 2
#define LOD 3
#define STO 4
#define CAL 5
#define INC 6
#define JMP 7
#define JPC 8
#define WRT 9
#define RED 10
#define HLT 11
#define NEG 12
#define ADD 13
#define SUB 14
#define MUL 15
#define DIV 16
#define MOD 17
#define EQL 18
#define NEQ 19
#define LSS 20
#define LEQ 21
#define GTR 22
#define GEQ 23

// ISA opnames
char *opnames[23] = {
    "LIT",
    "RET",
    "LOD",
    "STO",
    "CAL",
    "INC",
    "JMP",
    "JPC",
    "WRT",
    "RED",
    "HLT",
    "NEG",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "EQL",
    "NEQ",
    "LSS",
    "LEQ",
    "GTR",
    "GEQ",
};

// Initiliaze stack and register file
int stack[MAX_STACK_LENGTH];
int RF[REG_FILE_SIZE];

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

    // keep this
    if (printFlag)
    {
        printf("\t\t\t\t\tPC\tSP\tBP\n");
        printf("Initial values:\t\t\t\t%d\t%d\t%d\n", PC, SP, BP);
    }

    // Enter fetch-execute cycle.
    int line, halt = 0;
    while (halt == 0)
    {
        // Fetch an instruction and place in IR register.
        // Increment program counter.
        line = PC;
        instruction IR;
        IR = code[PC++];

        // Temp variable for calculations
        int tmp = 0;

        // Execute instruction based off instruction opcode.
        switch (IR.opcode)
        {
        default:
            printf("bad opcode");
            break;

        //LIT
        case 1:
            // RF[IR.r] = IR.m
            RF[IR.r] = IR.m;
            break;

        //RET
        case 2:
            // Return from current procedure (X) to the last procedure (Y).
            // SP = the index of the end of Y’s AR (BP + 1)
            // BP = dynamic link value from X’s AR
            // PC = return address value from X’s AR
            SP = BP + 1;
            BP = stack[SP - 2];
            PC = stack[SP - 3];
            break;

        //LOD
        case 3:
            // Load value to register IR.R from the stack location at
            // offset RF[IR.M] from L lexicographical levels up
            // Before performing the load, check for Out of Bounds
            // Access Error by checking if base(L) - M is less than
            // zero or greater than or equal to MAX_STACK_LENGTH
            tmp = base(IR.l, BP, stack) - RF[IR.m];
            if(tmp < 0 || tmp >= MAX_STACK_LENGTH){
                printf("Virtual Machine Error: Out of Bounds Access Error\n");
                halt=1;
            }
            else{
                RF[IR.r] = stack[tmp];
            }
            break;

        //STO
        case 4:
            // Store value from register IR.R to the stack location at
            // offset RF[IR.M] from L lexicographical levels down
            // Before performing the load, check for Out of Bounds
            // Access Error by checking if base(L) - M is less than
            // zero or greater than or equal to MAX_STACK_LENGTH
            tmp = base(IR.l, BP, stack) - RF[IR.m];
            if(tmp < 0 || tmp >= MAX_STACK_LENGTH){
                printf("Virtual Machine Error: Out of Bounds Access Error\n");
                halt=1;
            }
            else{
                stack[tmp] = RF[IR.r];
            }
            break;
            
        //CAL
        case 5:
            // Call procedure at code index M. This will generate a
            // new Activation Record. There are three values in the
            // AR:
            //  1st - static link = base(L)
            //  2nd - dynamic link = BP
            //  3rd - return address = PC
            // After creating the activation record,
            //  BP = the index of the first entry of the new AR
            //  PC = IR.M
            stack[SP - 1] = base(IR.l, BP, stack);
            stack[SP - 2] = BP;
            stack[SP - 3] = PC;
            BP = SP - 1;
            PC = IR.m;
            break;

        //INC
        case 6:
            // Decrement SP by M, check for Stack Overflow Error
            // which can occur if SP < 0 after the decrement
            // if Stack Overflow exists, set halt to true
            SP = SP - IR.m;
            if (SP < 0){
                printf("Virtual Machine Error: Stack Overflow Error\n");
                halt = 1;
            }
            break;

        //JMP
        case 7:
            // Jump to instruction M
            PC = IR.m;
            break;

        //JPC
        case 8:
            // Jump to instruction M if register R is 0
            if (RF[IR.r] == 0)
                PC = IR.m;
            break;

        //WRT
        case 9:
            // Print register R
            tmp = RF[IR.r];
            printf("Write Value: %d\n", tmp);
            break;

        //RED
        case 10:
            // Register R equals scanf()
            printf("Please Enter a Value: ");
            scanf("%d", &tmp);
            printf("\n");
            RF[IR.r] = tmp;
            break;

        //HLT
        case 11:
            // End of program (Set Halt flag to true)
            halt = 1;
            break;

        //NEG
        case 12:
            // Negate the register R
            RF[IR.r] = -1 * RF[IR.r];
            break;

        //ADD
        case 13:
            // Add the registers L and M and store the result
            // in register R
            tmp = RF[IR.l] + RF[IR.m];
            RF[IR.r] = tmp;
            break;

        //SUB
        case 14:
            // Subtract register M from register L and store the result
            // in register R
            tmp = RF[IR.l] - RF[IR.m];
            RF[IR.r] = tmp;
            break;

        //MUL
        case 15:
            // Multiply registers L and M and store the result
            // in register R
            tmp = RF[IR.l] * RF[IR.m];
            RF[IR.r] = tmp;
            break;

        //DIV
        case 16:
            // Divide register L by register M and store the result
            // in register R
            tmp = RF[IR.l] / RF[IR.m];
            RF[IR.r] = tmp;
            break;

        //MOD
        case 17:
            // Set register R equal to register L modulo register M
            tmp = RF[IR.l] % RF[IR.m];
            RF[IR.r] = tmp;
            break;

        //EQL
        case 18:
            // If register L equals register M, set register R to 1.
            // Otherwise set register R to 0
            if (RF[IR.l] == RF[IR.m])
                RF[IR.r] = 1;
            else
                RF[IR.r] = 0;
            break;

        //NEQ
        case 19:
            // If register L does not equal register M, set register R to
            // 1. Otherwise set register R to 0
            if (RF[IR.l] != RF[IR.m])
                RF[IR.r] = 1;
            else
                RF[IR.r] = 0;
            break;

        //LSS
        case 20:
            // If register L is less than register M, set register R to 1.
            // Otherwise set register R to 0
            if (RF[IR.l] < RF[IR.m])
                RF[IR.r] = 1;
            else
                RF[IR.r] = 0;
            break;

        //LEQ
        case 21:
            // If register L is less than or equal to register M, set register R to 1.
            // Otherwise set register R to 0
            if (RF[IR.l] <= RF[IR.m])
                RF[IR.r] = 1;
            else
                RF[IR.r] = 0;
            break;

        //GTR
        case 22:
            // If register L is greater than register M, set register R to  1.
            // Otherwise set register R to 0
            if (RF[IR.l] > RF[IR.m])
                RF[IR.r] = 1;
            else
                RF[IR.r] = 0;
            break;

        //GEQ
        case 23:
            // If register L is greater than or equal to register M, set register R to 1.
            // Otherwise set register R to 0
            if (RF[IR.l] >= RF[IR.m])
                RF[IR.r] = 1;
            else
                RF[IR.r] = 0;
            break;
        }

        // Print line of execution after instruction
        // only if there was no error
        if(printFlag == 1 && (halt == 0 || IR.opcode == 11)){
            char *op = opnames[IR.opcode - 1];
            print_execution(line, op, IR, PC, BP, SP, stack, RF);
        }
    }
}
