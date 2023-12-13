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
  uint32_t pc                = start_addr;
  bool     running           = true;
  long int instruction_count = 0;

  initialize_decode_functions(&decode_functions[0]);
  initialize_execute_functions(&execute_functions[0]);

  printf("Starting simulation at address %08x\n", start_addr);

  while (running) {

    uint32_t instruction = fetch_instruction(mem, pc);
    uint32_t opcode      = instruction & 0x7f;
    if (opcode == 0x0) {
      printf("Opcode is 0, exiting..\n");
      running = false;
      break;
    }
    printf("Opcode: %08x\n", opcode);
    void*     decoded_instruction = decode_functions[opcode](instruction);
    payload_t payload             = {registers, &pc};
    execute_functions[opcode](decoded_instruction, mem, &payload);

    if (log_file != NULL) {
      fprintf(log_file, "PC: %08x, Instruction: %08x\n", pc, instruction);
    }

    printf("Instruction count: %ld\n", instruction_count);
    printf("PC: %08x, Instruction: %08x\n", pc, instruction);
    pc += 4;
    instruction_count++;
  }
  return instruction_count;
  printf("DONE SIMULATE\n");
}

// Function to fetch an instruction from memory
uint32_t fetch_instruction(struct memory* mem, int address) {
  uint32_t instruction = 0;
  instruction |= memory_rd_b(mem, address + 0) << 0;
  instruction |= memory_rd_b(mem, address + 1) << 8;
  instruction |= memory_rd_b(mem, address + 2) << 16;
  instruction |= memory_rd_b(mem, address + 3) << 24;
  return instruction;
}
