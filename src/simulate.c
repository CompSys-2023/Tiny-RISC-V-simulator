#include "simulate.h" //maybe delete
#include "assembly.h"
#include "decode.h"
#include "execute.h"
#include "macros.h"
#include "memory.h"
#include "test.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1 for testing, 0 for not testing (only ran on fib.dis otherwise turn off)
#define TEST_MODE 1

const long int max_instructions = 100000;

decode_fn_ptr decode_functions[OPCODE_FUNCTION_ARRAY_SIZE]  = {0};
exec_fn_ptr   execute_functions[OPCODE_FUNCTION_ARRAY_SIZE] = {0};

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  int32_t  pc                       = start_addr;
  long int instruction_count        = 0;
  int32_t  registers[NUM_REGISTERS] = {0};

#if TEST_MODE
  int tests_passed = 0;
  int tests_total  = 0;
#endif

  initialize_decode_functions(&decode_functions[0]);
  initialize_execute_functions(&execute_functions[0]);

  printf("Starting simulation at address %08x\n", start_addr);

  while (instruction_count < max_instructions != 0) {
    int       instruction         = memory_rd_w(mem, pc);
    int       opcode              = instruction & 0x7f;
    int       pc_before           = pc;
    void*     decoded_instruction = decode_functions[opcode](instruction);
    payload_t payload             = {registers, &pc};

#if TEST_MODE
    int32_t previous_reg[NUM_REGISTERS];
    memcpy(previous_reg, registers, sizeof(registers));
#endif

    execute_functions[opcode](decoded_instruction, mem, &payload);

    if (log_file != NULL) {
      fprintf(log_file, "PC: %05x, Instruction: %08x\n", pc_before,
              instruction);
    }

    if (pc_before == pc) {
      pc += 4;
    }

#if TEST_MODE
    test_t test_data = {previous_reg, registers, pc_before,
                        pc,           mem,       instruction_count};
    tests_passed += test(test_data);
    tests_total++;
#endif

    instruction_count++;
  }

#if TEST_MODE
  printf(TEST_SUMMARY_FORMAT, tests_passed, tests_total);
#endif
  return instruction_count;
}
