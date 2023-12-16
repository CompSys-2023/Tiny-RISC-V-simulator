#ifndef EXECUTE_H
#define EXECUTE_H

#include "macros.h"
#include "memory.h"
#include <stdint.h>

typedef struct {
  int32_t*  x;
  uint32_t* pc;
} payload_t;

typedef void (*exec_fn_ptr)(void* instr, struct memory* mem, payload_t* p);
void initialize_execute_functions(exec_fn_ptr*);
void execute_R_type(void* instr, struct memory* mem, payload_t* payload);
void execute_I_type(void* instr, struct memory* mem, payload_t* payload);
void execute_S_type(void* instr, struct memory* mem, payload_t* payload);
void execute_B_type(void* instr, struct memory* mem, payload_t* payload);
void execute_J_type(void* instr, struct memory* mem, payload_t* payload);
void execute_U_type(void* instr, struct memory* mem, payload_t* payload);
#endif