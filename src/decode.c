+#include "decode.h"

void initializeDecodeFunctions(DecodeFunction* decode_functions) {
  decode_functions[R_TYPE_OPCODE] = &decodeRType;
  decode_functions[I_TYPE_OPCODE] = &decodeIType;
  decode_functions[S_TYPE_OPCODE] = &decodeSType;
  decode_functions[B_TYPE_OPCODE] = &decodeBType;
  decode_functions[U_TYPE_OPCODE] = &decodeUType;
  decode_functions[J_TYPE_OPCODE] = &decodeJType;
}

RTypeInstruction decodeRType(uint32_t instruction) {
  RTypeInstruction decoded;
  decoded.opcode = instruction & 0x7f;
  decoded.rd     = (instruction >> 7) & 0x1f;
  decoded.funct3 = (instruction >> 12) & 0x7;
  decoded.rs1    = (instruction >> 15) & 0x1f;
  decoded.rs2    = (instruction >> 20) & 0x1f;
  decoded.funct7 = (instruction >> 25) & 0x7f;
  return decoded;
}
ITypeInstruction decodeIType(uint32_t instruction) {
  ITypeInstruction decoded;
  decoded.opcode = instruction & 0x7f;
  decoded.rd     = (instruction >> 7) & 0x1f;
  decoded.funct3 = (instruction >> 12) & 0x7;
  decoded.rs1    = (instruction >> 15) & 0x1f;
  decoded.imm    = (instruction >> 20) & 0xfff;
  return decoded;
}
STypeInstruction decodeSType(uint32_t instruction) {
  STypeInstruction decoded;
  decoded.opcode   = instruction & 0x7f;
  decoded.imm_4_0  = (instruction >> 7) & 0x1f;
  decoded.funct3   = (instruction >> 12) & 0x7;
  decoded.rs1      = (instruction >> 15) & 0x1f;
  decoded.rs2      = (instruction >> 20) & 0x1f;
  decoded.imm_11_5 = (instruction >> 25) & 0x7f;
  return decoded;
}
BTypeInstruction decodeBType(uint32_t instruction) {
  BTypeInstruction decoded;
  decoded.opcode   = instruction & 0x7f;
  decoded.imm_11   = (instruction >> 7) & 0x1;
  decoded.imm_4_1  = (instruction >> 8) & 0xf;
  decoded.funct3   = (instruction >> 12) & 0x7;
  decoded.rs1      = (instruction >> 15) & 0x1f;
  decoded.rs2      = (instruction >> 20) & 0x1f;
  decoded.imm_10_5 = (instruction >> 25) & 0x3f;
  decoded.imm_12   = (instruction >> 31) & 0x1;
  return decoded;
}
UTypeInstruction decodeUType(uint32_t instruction) {
  UTypeInstruction decoded;
  decoded.opcode = instruction & 0x7f;
  decoded.rd     = (instruction >> 7) & 0x1f;
  decoded.imm    = (instruction >> 12) & 0xfffff;
  return decoded;
}
JTypeInstruction decodeJType(uint32_t instruction) {
  JTypeInstruction decoded;
  decoded.opcode    = instruction & 0x7f;
  decoded.rd        = (instruction >> 7) & 0x1f;
  decoded.imm_19_12 = (instruction >> 12) & 0xff;
  decoded.imm_11    = (instruction >> 20) & 0x1;
  decoded.imm_10_1  = (instruction >> 21) & 0x3ff;
  decoded.imm_20    = (instruction >> 31) & 0x1;
  return decoded;
}
