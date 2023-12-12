#ifndef DECODE_H
#define DECODE_H
#include "macros.h"
#include "simulate.h"
#include <stdint.h>

typedef void* (*decode_function_ptr)(uint32_t instruction);
void initialize_decode_functions(decode_function_ptr* functions);
void decode_R_type(uint32_t instruction);
void decode_I_type(uint32_t instruction);
void decode_S_type(uint32_t instruction);
void decode_B_type(uint32_t instruction);
void decode_U_type(uint32_t instruction);
void decode_J_type(uint32_t instruction);
#endif