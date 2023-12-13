#ifndef DECODE_H
#define DECODE_H
#include "macros.h"
#include "simulate.h"
#include <stdint.h>

typedef void* (*decode_fn_ptr)(uint32_t instr);
void                initialize_decode_functions(decode_fn_ptr* functions);
rtype_instruction_t decode_R_type(uint32_t instruction);
itype_instruction_t decode_I_type(uint32_t instruction);
stype_instruction_t decode_S_type(uint32_t instruction);
btype_instruction_t decode_B_type(uint32_t instruction);
utype_instruction_t decode_U_type(uint32_t instruction);
jtype_instruction_t decode_J_type(uint32_t instruction);
#endif