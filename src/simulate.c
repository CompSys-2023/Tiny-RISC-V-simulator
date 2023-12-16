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
       ? (printf("[PASSED]: `%s`: Expected: %d, Actual: %d\n", message,        \
                 (expected), (actual)),                                        \
          1)                                                                   \
       : (printf("[FAILED]: `%s`: Expected: %d, Actual: %d\n", message,        \
                 (expected), (actual)),                                        \
          0))

#define ASSERT_MULTIPLE(test1, test2) ((test1) && (test2))
#endif

const long int max_instructions = 7;

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
    uint32_t  pc_before           = pc;

#if TEST_MODE
    int32_t previous_reg[NUM_REGISTERS];
    memcpy(previous_reg, registers, sizeof(registers));
#endif

    execute_functions[opcode](decoded_instruction, mem, &payload);

    // printf("[%d]: PC: %05x, Instruction: %08x\n", instruction_count, pc,
    // instruction);
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

#if TEST_MODE

// expected er altid formentlig højst sandsynlig nok `x_prev` register
// actual er altid `x` register
int test(test_t test_data) {
  int32_t*       x_before  = test_data.x_before;
  int32_t*       x_after   = test_data.x_after;
  int            pc_before = test_data.pc_before;
  int            pc_after  = test_data.pc_after;
  struct memory* mem       = test_data.mem;
  int32_t        instr     = test_data.instr;

  switch (instr) {
    case 0: {
      int32_t expected = 0x1000 << 12;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "lui a0, 0x1000");
    }
    case 1: {
      int32_t expected = x_before[REG_SP] + x_before[REG_A0];
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "mv	sp,a0");
    }
    // skip 2 (lui)
    case 3: {
      int32_t expected = pc_before + (x_before[REG_RA] << 12);
      return ASSERT_EQUAL_INT(expected, x_after[REG_RA], "auipc ra,0x0");
    }
    case 4: {
      int32_t expected_ra = pc_before + 4;
      int     result1 =
          ASSERT_EQUAL_INT(expected_ra, x_after[REG_RA], "jalr	904(ra)");

      int32_t expected_2 = x_before[REG_RA] + 904;
      int     result2 = ASSERT_EQUAL_INT(expected_2, pc_after, "jalr	904(ra)");

      return ASSERT_MULTIPLE(result1, result2);
    }
    case 5: {
      int32_t expected = x_before[REG_SP] - 32;
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "addi	sp,sp,-32");
    }
    case 6: {
      printf("mem function poitner %p\n", mem);
      int32_t expected = memory_rd_w(mem, x_before[REG_SP] + 28);
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "sw	a0,28(sp)");
    }
  }
  return 0;
}
#endif
//