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

#define TEST_MODE 1 // 1 for testing, 0 for not testingtests_total

#if TEST_MODE
#define ASSERT_EQUAL_INT(expected, actual, message)                            \
  (((expected) == (actual))                                                    \
       ? (printf("Test passed: %s\n", message), 1)                             \
       : (printf("Test failed: %s. Expected: %d, Actual: %d\n", message,       \
                 (expected), (actual)),                                        \
          0))

#endif

const long int max_instructions = 1;

decode_fn_ptr decode_functions[OPCODE_FUNCTION_ARRAY_SIZE]  = {0};
exec_fn_ptr   execute_functions[OPCODE_FUNCTION_ARRAY_SIZE] = {0};
int32_t       registers[NUM_REGISTERS]                      = {0};

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  uint32_t pc                = start_addr;
  long int instruction_count = 0;

#if TEST_MODE
  int tests_passed = 0;
  int tests_total  = 0;
#endif

  initialize_decode_functions(&decode_functions[0]);
  initialize_execute_functions(&execute_functions[0]);

  printf("Starting simulation at address %08x\n", start_addr);

  while (1 && instruction_count < max_instructions) {
    uint32_t  instruction         = memory_rd_w(mem, pc);
    int32_t   opcode              = instruction & 0x7f;
    void*     decoded_instruction = decode_functions[opcode](instruction);
    payload_t payload             = {registers, &pc};
    uint32_t  prev_pc             = pc;
#if TEST_MODE
    int* prev_reg = registers;
#endif
    execute_functions[opcode](decoded_instruction, mem, &payload);

    printf("[%d]: PC: %05x, Instruction: %08x\n", instruction_count, pc,
           instruction);
    if (log_file != NULL) {
      fprintf(log_file, "PC: %05x, Instruction: %08x\n", pc, instruction);
    }

    if (prev_pc == pc) {
      pc += 4;
    }

#if TEST_MODE
    tests_passed += test(registers, prev_reg, instruction_count);
    tests_total++;
#endif

    instruction_count++;
  }

  // make a test passed out of x print make it look good

#if TEST_MODE
  printf(TEST_SUMMARY_FORMAT, tests_passed, tests_total);
#endif
  return instruction_count;
}

#if TEST_MODE
int test(int32_t* x, int32_t* prev, int32_t instr) {
  switch (instr) {
    case 0: {
      int32_t expected = 0x1000 << 12;
      return ASSERT_EQUAL_INT(expected, x[REG_A0], "lui a0, 0x1000");
    }
    case 1: {
      
    }
   }
  return 0;
}
#endif
//