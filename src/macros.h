// opcodes
// max opcode + 1 since it needs to just range from zero to max opcode so we can
// index in O(1) time.

// registers
#define NUM_REGISTERS 32
#define REG_ZERO 0
#define REG_RA 1
#define REG_SP 2
#define REG_GP 3
#define REG_TP 4
#define REG_T0 5
#define REG_T1 6
#define REG_T2 7
#define REG_S0 8
#define REG_S1 9
#define REG_A0 10
#define REG_A1 11
#define REG_A2 12
#define REG_A3 13
#define REG_A4 14
#define REG_A5 15
#define REG_A6 16
#define REG_A7 17
#define REG_S2 18
#define REG_S3 19
#define REG_S4 20
#define REG_S5 21
#define REG_S6 22
#define REG_S7 23
#define REG_S8 24
#define REG_S9 25
#define REG_S10 26
#define REG_S11 27
#define REG_T3 28
#define REG_T4 29
#define REG_T5 30
#define REG_T6 31

// ██████████████████████████████████████████
// █▄─▄▄▀███▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// ██─▄─▄██████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▄▄▀▄▄▀▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
#define R_TYPE_OPCODE 0x33

// ADD | SUB | MUL
#define FUNCT3_ADD_SUB_MUL 0x0
#define FUNCT7_ADD 0x00
#define FUNCT7_SUB 0x20
#define FUNCT7_MUL 0x01

// XOR | DIV
#define FUNCT3_XOR_DIV 0x4
#define FUNCT7_XOR 0x00
#define FUNCT7_DIV 0x01

// OR | REM
#define FUNCT3_OR_REM 0x6
#define FUNCT7_OR 0x00
#define FUNCT7_REM 0x01

// AND | REMU
#define FUNCT3_AND_REMU 0x7
#define FUNCT7_AND 0x00
#define FUNCT7_REMU 0x01

// SLL | MULH
#define FUNCT3_SLL_MULH 0x1
#define FUNCT7_SLL 0x00
#define FUNCT7_MULH 0x01

// SRL | SRA | DIVU
#define FUNCT3_SRL_SRA_DIVU 0x5
#define FUNCT7_SRL 0x00
#define FUNCT7_SRA 0x20
#define FUNCT7_DIVU 0x01

// SLT | MULSU
#define FUNCT3_SLT_MULSU 0x2
#define FUNCT7_SLT 0x00
#define FUNCT7_MULSU 0x01

// SLTU | MULU
#define FUNCT3_SLTU_MULU 0x3
#define FUNCT7_SLTU 0x00
#define FUNCT7_MULU 0x01

// ████████████████████████████████████████
// █▄─▄███▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// ██─███████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▄▄▄▀▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
#define I_TYPE_OPCODE 0x13

// ADDI
#define FUNCT3_ADDI 0x0

// XORI
#define FUNCT3_XORI 0x4

// ORI
#define FUNCT3_ORI 0x6

// ANDI
#define FUNCT3_ANDI 0x7

// SLLI
#define FUNCT3_SLLI 0x1

// SRLI | SRAI
#define FUNCT3_SRLI_SRAI 0x5
#define FUNCT7_SRLI 0x00 // imm[5:11]=0x00
#define FUNCT7_SRAI 0x20 // imm[5:11]=0x20

// SLTI
#define FUNCT3_SLTI 0x2

// SLTIU
#define FUNCT3_SLTIU 0x3

// ████████████████████████████████████████████████████████████████████████
// █▄─▄███▀▀▀▀▀██▄─▄███─▄▄─██▀▄─██▄─▄▄▀███▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// ██─████████████─██▀█─██─██─▀─███─██─██████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▄▄▄▀▀▀▀▀▀▀▀▀▀▄▄▄▄▄▀▄▄▄▄▀▄▄▀▄▄▀▄▄▄▄▀▀▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
#define I_TYPE_OPCODE_LOAD 0x3

// LB
#define FUNCT3_LB 0x0

// LH
#define FUNCT3_LH 0x1

// LW
#define FUNCT3_LW 0x2

// LBU
#define FUNCT3_LBU 0x4

// LHU
#define FUNCT3_LHU 0x5

// ██████████████████████████████████████████
// █─▄▄▄▄███▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// █▄▄▄▄─██████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▄▄▄▄▄▀▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
#define S_TYPE_OPCODE 0x23

//  SB
#define FUNCT3_SB 0x0

// SH
#define FUNCT3_SH 0x1

// SW
#define FUNCT3_SW 0x2

// ██████████████████████████████████████████
// █▄─▄─▀███▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// ██─▄─▀██████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▄▄▄▄▀▀▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
#define B_TYPE_OPCODE 0x63

// BEQ
#define FUNCT3_BEQ 0x0

// BNE
#define FUNCT3_BNE 0x1

// BLT
#define FUNCT3_BLT 0x4

// BGE
#define FUNCT3_BGE 0x5

// BLTU
#define FUNCT3_BLTU 0x6

// BGEU
#define FUNCT3_BGEU 0x7

// ██████████████████████████████████████████
// ███▄─▄███▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// █─▄█─███████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▄▄▄▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
#define J_TYPE_OPCODE 0x6f

// JAL
#define JAL_OPCODE J_TYPE_OPCODE

// JALR (decoding is I-type, but unique opcode)
#define I_TYPE_OPCODE_JALR 0x67

// ███████████████████████████████████████████
// █▄─██─▄███▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// ██─██─███████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▀▄▄▄▄▀▀▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
#define U_TYPE_OPCODE_LUI 0x37
#define U_TYPE_OPCODE_AUI 0x17

// LUI
#define LUI_OPCODE U_TYPE_OPCODE_LUI

// AUI
#define AUIPC_OPCODE U_TYPE_OPCODE_AUI

// ███████████████████████████████████████████████████████████████████
// █▄─▄███▀▀▀▀▀████─▄▄▄▄█▄─█─▄█─▄▄▄▄█▀▀▀▀▀████─▄─▄─█▄─█─▄█▄─▄▄─█▄─▄▄─█
// ██─█████████████▄▄▄▄─██▄─▄██▄▄▄▄─████████████─████▄─▄███─▄▄▄██─▄█▀█
// ▀▄▄▄▀▀▀▀▀▀▀▀▀▀▀▀▄▄▄▄▄▀▀▄▄▄▀▀▄▄▄▄▄▀▀▀▀▀▀▀▀▀▀▀▄▄▄▀▀▀▄▄▄▀▀▄▄▄▀▀▀▄▄▄▄▄▀
// ECALL (decoding I-type but different opcodes, but environment syscall)
#define I_TYPE_OPCODE_ECALL 0x73
#define OPCODE_FUNCTION_ARRAY_SIZE (I_TYPE_OPCODE_ECALL + 1)

// █████████████████████████████████████████▀█
// █─▄─▄─█▄─▄▄─█─▄▄▄▄█─▄─▄─█▄─▄█▄─▀█▄─▄█─▄▄▄▄█
// ███─████─▄█▀█▄▄▄▄─███─████─███─█▄▀─██─██▄─█
// ▀▀▄▄▄▀▀▄▄▄▄▄▀▄▄▄▄▄▀▀▄▄▄▀▀▄▄▄▀▄▄▄▀▀▄▄▀▄▄▄▄▄▀

#define TEST_SUMMARY_FORMAT                                                    \
  "\n============== TESTS OVERVIEW ==============\n"                           \
  "Passed %d out of 43 tests.\n"                                               \
  "============================================\n"