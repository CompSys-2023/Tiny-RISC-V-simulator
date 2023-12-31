#ifndef EXECUTE_H
#define EXECUTE_H

#include "macros.h"
#include "memory.h"
#include <stdint.h>

typedef struct {
  int32_t* regs;
  int32_t* pc;
} payload_t;

typedef void (*exec_fn_ptr)(void* instr, struct memory* mem, payload_t* p);
void initialize_execute_functions(exec_fn_ptr*);
void execute_R_type(void* instr, struct memory* mem, payload_t* payload);
void execute_I_type(void* instr, struct memory* mem, payload_t* payload);
void execute_S_type(void* instr, struct memory* mem, payload_t* payload);
void execute_B_type(void* instr, struct memory* mem, payload_t* payload);
void execute_J_type(void* instr, struct memory* mem, payload_t* payload);
void execute_U_type(void* instr, struct memory* mem, payload_t* payload);

// branch logic
typedef int (*branch_func_t)(int32_t, int32_t);
int beq(int32_t rs1, int32_t rs2);
int bne(int32_t rs1, int32_t rs2);
int blt(int32_t rs1, int32_t rs2);
int bge(int32_t rs1, int32_t rs2);
int bltu(int32_t rs1, int32_t rs2);
int bgeu(int32_t rs1, int32_t rs2);

// store logic
typedef void (*store_func_t)(struct memory*, int32_t, int);

// load logic
typedef int (*load_func_t)(struct memory*, int32_t);

#endif