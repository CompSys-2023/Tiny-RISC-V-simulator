#ifndef EXECUTE_H
#define EXECUTE_H
#include "macros.h"

typedef void (*execute_function_ptr)(void* instruction, struct memory* mem);
void initialize_execute_functions(execute_function_ptr* functions);
void execute_R_type(void* instruction, struct memory* mem);
void execute_I_type(void* instruction, struct memory* mem);
void execute_S_type(void* instruction, struct memory* mem);
void execute_B_type(void* instruction, struct memory* mem);
void execute_U_type(void* instruction, struct memory* mem);
void execute_J_type(void* instruction, struct memory* mem);
#endif