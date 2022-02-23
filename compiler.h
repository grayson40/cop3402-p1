/*
  This is the header file for Almalki's Spring 2022 Systems Software Project.
  If you choose to alter this, you MUST make a note of that in your
  readme file, otherwise you will lose 5 points.
*/

typedef enum token_type
{
  periodsym = 1,
  varsym,
  lbracketsym,
  procsym,
  rbracketsym,
  callsym,
  commasym,
  beginsym,
  semicolonsym,
  endsym,
  assignsym,
  ifsym,
  questionsym,
  dosym,
  colonsym,
  readsym,
  lparenthesissym,
  writesym,
  rparenthesissym,
  identsym,
  eqlsym,
  numbersym,
  neqsym,
  modsym,
  lsssym,
  divsym,
  leqsym,
  multsym,
  gtrsym,
  subsym,
  geqsym,
  addsym,
  whilesym
} token_type;

typedef struct lexeme
{
  char name[12];
  int value;
  token_type type;
} lexeme;

typedef struct instruction
{
  int opcode;
  int r;
  int l;
  int m;
} instruction;

typedef struct symbol
{
  int kind; // 1 for variables, 2 for arrays, 3 for procedures
  char name[12];
  int size; // length of array
  int level;
  int addr; // address for variables, first entry address for arrays, first instruction index for procedures
  int mark; // if the symbol is active
} symbol;

// Map opcodes integer value to name
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
#define HAL 11
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

char opname[23][3] = {
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
    "HAL",
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

lexeme *lexanalyzer(char *input, int printFlag);
instruction *parse(lexeme *list, int printTable, int printCode);
void execute_program(instruction *code, int printFlag);