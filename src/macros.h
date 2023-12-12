// opcodes
#define OPCODE_SIZE 7;
#define R_TYPE_OPCODE 0x33
#define I_TYPE_OPCODE 0x13
#define S_TYPE_OPCODE 0x23
#define B_TYPE_OPCODE 0x63
#define U_TYPE_OPCODE 0x37
#define J_TYPE_OPCODE 0x6f

// max opcode + 1 since it needs to just range from zero to max opcode so we can
// index in O(1) time.
#define OPCODE_FUNCTION_ARRAY_SIZE (J_TYPE_OPCODE + 1)

// registers
#define x0 0
#define x1 1
#define x2 2
#define x3 3
#define x4 4
#define x5 5
#define x6 6
#define x7 7
#define x8 8
#define x9 9
#define x10 10
#define x11 11
#define x12 12
#define x13 13
#define x14 14
#define x15 15
#define x16 16
#define x17 17
#define x18 18
#define x19 19
#define x20 20
#define x21 21
#define x22 22
#define x23 23
#define x24 24
#define x25 25
#define x26 26
#define x27 27
#define x28 28
#define x29 29
#define x30 30
#define x31 31

// funct3
