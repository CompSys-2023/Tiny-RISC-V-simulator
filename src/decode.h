#include "macros.h"
#include "simulate.h"

typedef void* (*DecodeFunction)(uint32_t instruction);
void             initializeDecodeFunctions(DecodeFunction* decode_functions);
RTypeInstruction decodeRType(uint32_t instruction);
ITypeInstruction decodeIType(uint32_t instruction);
STypeInstruction decodeSType(uint32_t instruction);
BTypeInstruction decodeBType(uint32_t instruction);
UTypeInstruction decodeUType(uint32_t instruction);
JTypeInstruction decodeJType(uint32_t instruction);
