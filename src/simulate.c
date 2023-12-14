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

const long int max_instructions = 50000;

decode_fn_ptr decode_functions[OPCODE_FUNCTION_ARRAY_SIZE]  = {0};
exec_fn_ptr   execute_functions[OPCODE_FUNCTION_ARRAY_SIZE] = {0};
int32_t       registers[NUM_REGISTERS]                      = {0};

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  uint32_t pc                = start_addr;
  bool     running           = true;
  long int instruction_count = 0;

  initialize_decode_functions(&decode_functions[0]);
  initialize_execute_functions(&execute_functions[0]);

  printf("Starting simulation at address %08x\n", start_addr);

  while (running && instruction_count < max_instructions) {
    uint32_t instruction = memory_rd_w(mem, pc);
    if (instruction == 0x0) {
      printf("NULL instruction\n");
      running = false;
      return instruction_count;
    }
    int32_t opcode = instruction & 0x7f;
    printf("Opcode: %x\n", opcode);
    void*     decoded_instruction = decode_functions[opcode](instruction);
    payload_t payload             = {registers, &pc};
    execute_functions[opcode](decoded_instruction, mem, &payload);

    if (log_file != NULL) {
      fprintf(log_file, "PC: %08x, Instruction: %08x\n", pc, instruction);
    }

    printf("Count: %ld\n", instruction_count);
    printf("PC: %08x, Instruction: %08x\n", pc, instruction);
    pc += 4;
    instruction_count++;
  }
  return instruction_count;
  printf("DONE SIMULATE\n");
}
