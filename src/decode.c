#include "decode.h"

void initialize_decode_functions(decode_fn_ptr* functions) {
  functions[R_TYPE_OPCODE]       = &decode_R_type;
  functions[I_TYPE_OPCODE]       = &decode_I_type;
  functions[I_TYPE_OPCODE_ECALL] = &decode_I_type;
  functions[I_TYPE_OPCODE_JALR]  = &decode_I_type;
  functions[I_TYPE_OPCODE_LOAD]  = &decode_I_type;
  functions[S_TYPE_OPCODE]       = &decode_S_type;
  functions[B_TYPE_OPCODE]       = &decode_B_type;
  functions[J_TYPE_OPCODE]       = &decode_J_type;
  functions[U_TYPE_OPCODE_LUI]   = &decode_U_type;
  functions[U_TYPE_OPCODE_AUI]   = &decode_U_type;
}

rtype_instruction_t* decode_R_type(uint32_t instruction) {
  rtype_instruction_t* decoded = malloc(sizeof(rtype_instruction_t));
  decoded->opcode              = instruction & 0x7f;
  decoded->rd                  = (instruction >> 7) & 0x1f;
  decoded->funct3              = (instruction >> 12) & 0x7;
  decoded->rs1                 = (instruction >> 15) & 0x1f;
  decoded->rs2                 = (instruction >> 20) & 0x1f;
  decoded->funct7              = (instruction >> 25) & 0x7f;
  return decoded;
}

itype_instruction_t* decode_I_type(uint32_t instruction) {
  itype_instruction_t* decoded = malloc(sizeof(itype_instruction_t));
  decoded->opcode              = instruction & 0x7f;
  decoded->rd                  = (instruction >> 7) & 0x1f;
  decoded->funct3              = (instruction >> 12) & 0x7;
  decoded->rs1                 = (instruction >> 15) & 0x1f;
  decoded->imm                 = (instruction >> 20) & 0xfff;
  return decoded;
}

stype_instruction_t* decode_S_type(uint32_t instruction) {
  stype_instruction_t* decoded = malloc(sizeof(stype_instruction_t));
  decoded->opcode              = instruction & 0x7f;
  decoded->imm_4_0             = (instruction >> 7) & 0x1f;
  decoded->funct3              = (instruction >> 12) & 0x7;
  decoded->rs1                 = (instruction >> 15) & 0x1f;
  decoded->rs2                 = (instruction >> 20) & 0x1f;
  decoded->imm_11_5            = (instruction >> 25) & 0x7f;
  return decoded;
}

btype_instruction_t* decode_B_type(uint32_t instruction) {
  btype_instruction_t* decoded = malloc(sizeof(btype_instruction_t));
  decoded->opcode              = instruction & 0x7f;
  decoded->imm_11              = (instruction >> 7) & 0x1;
  decoded->imm_4_1             = (instruction >> 8) & 0xf;
  decoded->funct3              = (instruction >> 12) & 0x7;
  decoded->rs1                 = (instruction >> 15) & 0x1f;
  decoded->rs2                 = (instruction >> 20) & 0x1f;
  decoded->imm_10_5            = (instruction >> 25) & 0x3f;
  decoded->imm_12              = (instruction >> 31) & 0x1;
  return decoded;
}

jtype_instruction_t* decode_J_type(uint32_t instruction) {
  jtype_instruction_t* decoded = malloc(sizeof(jtype_instruction_t));
  decoded->opcode              = instruction & 0x7f;
  decoded->rd                  = (instruction >> 7) & 0x1f;
  decoded->imm_19_12           = (instruction >> 12) & 0xff;
  decoded->imm_11              = (instruction >> 20) & 0x1;
  decoded->imm_10_1            = (instruction >> 21) & 0x3ff;
  decoded->imm_20              = (instruction >> 31) & 0x1;
  return decoded;
}

utype_instruction_t* decode_U_type(uint32_t instruction) {
  utype_instruction_t* decoded = malloc(sizeof(utype_instruction_t));
  decoded->opcode              = instruction & 0x7f;
  decoded->rd                  = (instruction >> 7) & 0x1f;
  decoded->imm                 = (instruction >> 12) & 0xfffff;
  return decoded;
}
