#ifndef __SIMULATE_H__
#define __SIMULATE_H__

#include "assembly.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t funct7 : 7;
} RTypeInstruction;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  int32_t  imm : 12;
} ITypeInstruction;

typedef struct {
  uint32_t opcode : 7;
  uint32_t imm_4_0 : 5;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  int32_t  imm_11_5 : 7;
} STypeInstruction;

typedef struct {
  uint32_t opcode : 7;
  uint32_t imm_11 : 1;
  uint32_t imm_4_1 : 4;
  uint32_t funct3 : 3;
  uint32_t rs1 : 5;
  uint32_t rs2 : 5;
  uint32_t imm_10_5 : 6;
  uint32_t imm_12 : 1;
} BTypeInstruction;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  int32_t  imm : 20;
} UTypeInstruction;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t imm_19_12 : 8;
  uint32_t imm_11 : 1;
  uint32_t imm_10_1 : 10;
  uint32_t imm_20 : 1;
} JTypeInstruction;

typedef enum {
  R_TYPE_OPCODE = 0x33, // 0011 0011 => 0x33
  I_TYPE_OPCODE = 0x13, // 0001 0011 => 0x13
  S_TYPE_OPCODE = 0x23, // 0010 0011 => 0x23
  B_TYPE_OPCODE = 0x63, // 0110 0011 => 0x63
  U_TYPE_OPCODE = 0x37, // 0011 0111 => 0x37
  J_TYPE_OPCODE = 0x6f, // 0110 1111 => 0x6f
} opcode_t;

typedef void* (*DecodeFunction)(uint32_t instruction);
RTypeInstruction decodeRType(uint32_t instruction);
ITypeInstruction decodeIType(uint32_t instruction);
STypeInstruction decodeSType(uint32_t instruction);
BTypeInstruction decodeBType(uint32_t instruction);
UTypeInstruction decodeUType(uint32_t instruction);
JTypeInstruction decodeJType(uint32_t instruction);

void executeRType(RTypeInstruction instruction, struct memory* mem);
void executeIType(ITypeInstruction instruction, struct memory* mem);
void executeSType(STypeInstruction instruction, struct memory* mem);
void executeBType(BTypeInstruction instruction, struct memory* mem);
void executeUType(UTypeInstruction instruction, struct memory* mem);
void executeJType(JTypeInstruction instruction, struct memory* mem);

// Simuler RISC-V program i givet lager og fra given start adresse
long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file);

#endif
