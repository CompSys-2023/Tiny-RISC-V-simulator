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
#include <unistd.h>

const long int max_instructions = 1000;

decode_fn_ptr decode_functions[OPCODE_FUNCTION_ARRAY_SIZE]  = {0};
exec_fn_ptr   execute_functions[OPCODE_FUNCTION_ARRAY_SIZE] = {0};
int32_t       registers[NUM_REGISTERS]                      = {0};

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  uint32_t pc                = start_addr;
  long int instruction_count = 0;

  initialize_decode_functions(&decode_functions[0]);
  initialize_execute_functions(&execute_functions[0]);

  printf("Starting simulation at address %08x\n", start_addr);

  while (1 && instruction_count < max_instructions) {
    uint32_t instruction = memory_rd_w(mem, pc);
    int32_t  opcode      = instruction & 0x7f;
    printf("[%d]: PC: %05x, Instruction: %08x\n", instruction_count, pc,
           instruction);

    void*     decoded_instruction = decode_functions[opcode](instruction);
    payload_t payload             = {registers, &pc};
    uint32_t  prev_pc             = pc;
    execute_functions[opcode](decoded_instruction, mem, &payload);

    if (log_file != NULL) {
      fprintf(log_file, "PC: %05x, Instruction: %08x\n", pc, instruction);
    }

    if (prev_pc == pc) {
      pc += 4;
    }

    instruction_count++;
  }
  return instruction_count;
  printf("DONE SIMULATE\n");
}

void print_registers(int32_t regs[], int num_regs) {
  // Printing the first row (register names)
  for (int i = 0; i < num_regs; ++i) {
    printf("| x%d ", i);
  }
  printf("|\n");

  // Printing the horizontal separator
  for (int i = 0; i < num_regs; ++i) {
    printf("------");
  }
  printf("\n");

  // Printing the second row (register values)
  for (int i = 0; i < num_regs; ++i) {
    printf("| %d ",
           regs[i]); // Adjust the spacing depending on expected value range
  }
  printf("|\n");
}