#include "execute.h"
#include "simulate.h"

void initializeExecuteFunctions(ExecuteFunction* execute_functions) {
  execute_functions[R_TYPE_OPCODE] = &executeRType;
  execute_functions[I_TYPE_OPCODE] = &executeIType;
  execute_functions[S_TYPE_OPCODE] = &executeSType;
  execute_functions[B_TYPE_OPCODE] = &executeBType;
  execute_functions[U_TYPE_OPCODE] = &executeUType;
  execute_functions[J_TYPE_OPCODE] = &executeJType;
}

void executeRType(void* instruction, struct memory* mem) {
  RTypeInstruction decoded = *(RTypeInstruction*)instruction;
}
void executeIType(void* instruction, struct memory* mem) {
  ITypeInstruction decoded = *(ITypeInstruction*)instruction;
}
void executeSType(void* instruction, struct memory* mem) {
  STypeInstruction decoded = *(STypeInstruction*)instruction;
}
void executeBType(void* instruction, struct memory* mem) {
  BTypeInstruction decoded = *(BTypeInstruction*)instruction;
}
void executeUType(void* instruction, struct memory* mem) {
  UTypeInstruction decoded = *(UTypeInstruction*)instruction;
}
void executeJType(void* instruction, struct memory* mem) {
  JTypeInstruction decoded = *(JTypeInstruction*)instruction;
  
}