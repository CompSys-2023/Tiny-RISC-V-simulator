// opcodes
#define OPCODE_SIZE 7;
#define R_TYPE_OPCODE 0x33
#define I_TYPE_OPCODE 0x13
#define S_TYPE_OPCODE 0x23
#define B_TYPE_OPCODE 0x63
#define U_TYPE_OPCODE_LUI 0x37
#define U_TYPE_OPCODE_AUI 0x17
#define J_TYPE_OPCODE 0x6f

// 00010011
//  0X13

// max opcode + 1 since it needs to just range from zero to max opcode so we can
// index in O(1) time.
#define OPCODE_FUNCTION_ARRAY_SIZE (J_TYPE_OPCODE + 1)

// registers
#define NUM_REGISTERS 32

// R-type
#define ADD_FUNCT3 0x000
#define ADD_FUNCT7 0x0000000
#define SUB_FUNCT7 0x0000000
#define SLL_FUNCT3 0x001
#define SLT_FUNCT3 0x010
#define SLTU_FUNCT3 0x011
#define XOR_FUNCT3 0x100
#define SRL_SRA_FUNCT3 0x101
#define SRL_FUNCT7 0x0000000
#define SRA_FUNCT7 0x0100000
#define OR_FUNCT3 0x110
#define AND_FUNCT3 0x111

// I-type
#define JALR_ADDI_FUNCT3 0x000
#define JALR_FUNCT7 0x0000000

// #define ADDI_FUNCT3 0x000
#define SLTI_FUNCT3 0x010
#define SLTIU_FUNCT3 0x011
#define XORI_FUNCT3 0x100
#define ORI_FUNCT3 0x110
#define ANDI_FUNCT3 0x111
#define SLLI_FUNCT3 0x001
#define SRLI_FUNCT3 0x101
#define SRAI_FUNCT3 0x101

// Load instructions
#define LB_FUNCT3 0x000
#define LH_FUNCT3 0x001
#define LW_FUNCT3 0x010
#define LBU_FUNCT3 0x100
#define LHU_FUNCT3 0x101

// S-type
#define SB_FUNCT3 0x000
#define SH_FUNCT3 0x001
#define SW_FUNCT3 0x010

// B-type
#define BEQ_FUNCT3 0x000
#define BNE_FUNCT3 0x001
#define BLT_FUNCT3 0x100
#define BGE_FUNCT3 0x101
#define BLTU_FUNCT3 0x110
#define BGEU_FUNCT3 0x111

// U-type this represents two different opcodes from above
#define LUI_OPCODE U_TYPE_OPCODE_LUI
#define AUIPC_OPCODE U_TYPE_OPCODE_AUI

// J-type
#define JAL_OPCODE J_TYPE_OPCODE