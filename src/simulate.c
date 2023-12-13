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

uint32_t      registers[NUM_REGISTERS]                      = {0};
decode_fn_ptr decode_functions[OPCODE_FUNCTION_ARRAY_SIZE]  = {0};
exec_fn_ptr   execute_functions[OPCODE_FUNCTION_ARRAY_SIZE] = {0};

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  int      pc                = start_addr;
  bool     running           = true;
  long int instruction_count = 0;

  assembly_delete(as);
  initialize_decode_functions(&decode_functions);
  initialize_execute_functions(&execute_functions);

  while (running) {
    uint32_t  instruction         = fetch_instruction(mem, pc);
    uint32_t  opcode              = instruction & 0x7f;
    void*     decoded_instruction = decode_functions[opcode](instruction);
    payload_t payload             = {registers, &pc};
    execute_functions[opcode](decoded_instruction, mem, &payload);

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
