#ifndef __SIMULATE_H__
#define __SIMULATE_H__

#include "assembly.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
  unsigned int opcode : 7;
  unsigned int rd : 5;
  unsigned int funct3 : 3;
  unsigned int rs1 : 5;
  unsigned int rs2 : 5;
  unsigned int funct7 : 7;
} rtype_instruction_t;

typedef struct {
  unsigned int opcode : 7;
  unsigned int rd : 5;
  unsigned int funct3 : 3;
  unsigned int rs1 : 5;
  signed int   imm : 12; // Signed for immediate values
} itype_instruction_t;

typedef struct {
  unsigned int opcode : 7;
  unsigned int funct3 : 3;
  unsigned int rs1 : 5;
  unsigned int rs2 : 5;
  signed int   imm : 12; // Signed for immediate values
} stype_instruction_t;

typedef struct {
  unsigned int opcode : 7;
  unsigned int funct3 : 3;
  unsigned int rs1 : 5;
  unsigned int rs2 : 5;
  signed int   imm : 12; // Signed for immediate values
} btype_instruction_t;

typedef struct {
  unsigned int opcode : 7;
  unsigned int rd : 5;
  signed int   imm : 20;
} jtype_instruction_t;

typedef struct {
  unsigned int opcode : 7;
  unsigned int rd : 5;
  signed int   imm : 20;
} utype_instruction_t;

// Simuler RISC-V program i givet lager og fra given start adresse
long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file);

void testing(signed int* x, signed int* prev, int pc_current, int pc_prev,
             signed int i);

#endif
