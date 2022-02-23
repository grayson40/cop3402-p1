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
