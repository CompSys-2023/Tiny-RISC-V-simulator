#include "simulate.h" //maybe delete
#include "assembly.h"
#include "decode.h"
#include "execute.h"
#include "macros.h"
#include "memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int                  registers[32]                                 = {0};
decode_function_ptr  decode_functions[OPCODE_FUNCTION_ARRAY_SIZE]  = {0};
execute_function_ptr execute_functions[OPCODE_FUNCTION_ARRAY_SIZE] = {0};

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  int      pc                = start_addr;
  bool     running           = true;
  long int instruction_count = 0;
  
  initialize_decode_functions(&decode_functions);
  initialize_execute_functions(&execute_functions);

  while (running) {
    uint32_t instruction         = fetch_instruction(mem, pc);
    uint32_t opcode              = instruction & 0x7f;
    void*    decoded_instruction = decode_functions[opcode](instruction);
    execute_functions[opcode](decoded_instruction, mem);

    if (log_file != NULL) {
      fprintf(log_file, "PC: %08x, Instruction: %08x\n", pc, instruction);
    }

    pc += 4;
    instruction_count++;
  }
  return instruction_count;
}

// Function to fetch an instruction from memory
uint32_t fetch_instruction(struct memory* mem, int address) {
  uint32_t instruction = 0;
  for (int i = 0; i < 4; i++) {
    instruction |= (memory_rd_b(mem, address + i) << (i * 8));
  }
  return instruction;
}
