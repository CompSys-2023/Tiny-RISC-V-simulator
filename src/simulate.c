#include "simulate.h"
#include "assembly.h"
#include "macros.h"
#include "memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int registers[32] = {0};

DecodeFunction decodeFunctions[J_TYPE_OPCODE + 1] = {0};
void           initializeDecodeFunctions() {
  decodeFunctions[R_TYPE_OPCODE] = &decodeRType;
  decodeFunctions[I_TYPE_OPCODE] = &decodeIType;
  decodeFunctions[S_TYPE_OPCODE] = &decodeSType;
  decodeFunctions[B_TYPE_OPCODE] = &decodeBType;
  decodeFunctions[U_TYPE_OPCODE] = &decodeUType;
  decodeFunctions[J_TYPE_OPCODE] = &decodeJType;
}

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  int      pc                = start_addr;
  bool     running           = true;
  long int instruction_count = 0;

  initializeDecodeFunctions();

  while (running) {
    uint32_t instruction = fetch_instruction(mem, pc);

    // Extract the opcode by masking the instruction with 0b1111111 (0x7f) which
    // will give us the last 7 bits of the instruction
    uint32_t opcode = instruction & 0x7f;

    // Decode the instruction
    void* decodedInstruction = decodeFunctions[opcode](instruction);

    // Execute the instruction
    execute_instruction(decodedInstruction, opcode, mem, &pc);

    // Logging the instruction
    if (log_file != NULL) {
      fprintf(log_file, "PC: %08x, Instruction: %08x\n", pc, instruction);
    }

    // Update the program counter and instruction count
    pc += 4;
    instruction_count++;
  }
  return instruction_count;
}

// Function to fetch an instruction from memory
uint32_t fetch_instruction(struct memory* mem, int address) { return 0; }

// Functions to decode instructions
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

// Function to execute an instruction
void execute_instruction(void* decoded_instruction, opcode_t opcode,
                         struct memory* mem, int* pc) {
  switch (opcode) {
    case R_TYPE_OPCODE:
      executeRType(*(RTypeInstruction*)decoded_instruction, mem);
      break;
    case I_TYPE_OPCODE:
      executeIType(*(ITypeInstruction*)decoded_instruction, mem);
      break;
    case S_TYPE_OPCODE:
      executeSType(*(STypeInstruction*)decoded_instruction, mem);
      break;
    case B_TYPE_OPCODE:
      executeBType(*(BTypeInstruction*)decoded_instruction, mem);
      break;
    case U_TYPE_OPCODE:
      executeUType(*(UTypeInstruction*)decoded_instruction, mem);
      break;
    case J_TYPE_OPCODE:
      executeJType(*(JTypeInstruction*)decoded_instruction, mem);
      break;
    default:
      break;
  }
}

void executeRType(RTypeInstruction instruction, struct memory* mem) {}
void executeIType(ITypeInstruction instruction, struct memory* mem) {}
void executeSType(STypeInstruction instruction, struct memory* mem) {}
void executeBType(BTypeInstruction instruction, struct memory* mem) {}
void executeUType(UTypeInstruction instruction, struct memory* mem) {}
void executeJType(JTypeInstruction instruction, struct memory* mem) {}