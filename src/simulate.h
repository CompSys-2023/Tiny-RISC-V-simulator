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
  int32_t  rs1 : 5;
  int32_t  rs2 : 5;
  uint32_t funct7 : 7;
} rtype_instruction_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  uint32_t funct3 : 3;
  int32_t  rs1 : 5;
  int32_t  imm : 12;
} itype_instruction_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t funct3 : 3;
  int32_t  rs1 : 5;
  int32_t  rs2 : 5;
  int32_t  imm : 12;
} stype_instruction_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t funct3 : 3;
  int32_t  rs1 : 5;
  int32_t  rs2 : 5;
  int32_t  imm : 12;
} btype_instruction_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  int32_t  imm : 20;
} utype_instruction_t;

typedef struct {
  uint32_t opcode : 7;
  uint32_t rd : 5;
  int32_t  imm : 20;
} jtype_instruction_t;

typedef struct {
  int32_t*       x_before;
  int32_t*       x_after;
  int32_t        pc_before;
  int32_t        pc_after;
  struct memory* mem;
  int32_t        instr_num;
} test_t;

// Simuler RISC-V program i givet lager og fra given start adresse
long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file);

void testing(int32_t* x, int32_t* prev, int pc_current, int pc_prev, int32_t i);

#endif
