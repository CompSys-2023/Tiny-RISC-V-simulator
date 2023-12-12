#include "execute.h"
#include "simulate.h"

void initialize_execute_functions(execute_function_ptr* functions) {
  functions[R_TYPE_OPCODE] = &execute_R_type;
  functions[I_TYPE_OPCODE] = &execute_I_type;
  functions[S_TYPE_OPCODE] = &execute_S_type;
  functions[B_TYPE_OPCODE] = &execute_B_type;
  functions[U_TYPE_OPCODE] = &execute_U_type;
  functions[J_TYPE_OPCODE] = &execute_J_type;
}

void execute_R_type(void* instruction, struct memory* mem) {
  rtype_instruction_t decoded = *(rtype_instruction_t*)instruction;
}

void execute_I_type(void* instruction, struct memory* mem) {
  itype_instruction_t decoded = *(itype_instruction_t*)instruction;
}

void execute_S_type(void* instruction, struct memory* mem) {
  stype_instruction_t decoded = *(stype_instruction_t*)instruction;
}

void execute_B_type(void* instruction, struct memory* mem) {
  btype_instruction_t decoded = *(btype_instruction_t*)instruction;
}

void execute_U_type(void* instruction, struct memory* mem) {
  utype_instruction_t decoded = *(utype_instruction_t*)instruction;
}

void execute_J_type(void* instruction, struct memory* mem) {
  jtype_instruction_t decoded = *(jtype_instruction_t*)instruction;
}