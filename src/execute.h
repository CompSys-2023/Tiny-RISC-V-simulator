#include "macros.h"

typedef void (*ExecuteFunction)(void* decodedInstruction, struct memory* mem);
void initializeExecuteFunctions(ExecuteFunction* execute_functions);
void executeRType(void* instruction, struct memory* mem);
void executeIType(void* instruction, struct memory* mem);
void executeSType(void* instruction, struct memory* mem);
void executeBType(void* instruction, struct memory* mem);
void executeUType(void* instruction, struct memory* mem);
void executeJType(void* instruction, struct memory* mem);
