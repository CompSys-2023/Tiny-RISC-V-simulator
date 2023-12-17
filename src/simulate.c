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

#define TEST_MODE 1 // 1 for testing, 0 for not testingtests_total

#if TEST_MODE
#define ASSERT_EQUAL_INT(expected, actual, message)                            \
  (((expected) == (actual))                                                    \
       ? (printf(                                                              \
              "\033[38;5;46m[\033[38;5;82m\033[1mPASSED\033[38;5;46m]\033[0m:" \
              "\033[38;5;82m `%s`: "                                           \
              "Expected: %d, "                                                 \
              "Actual: %d\n\033[0m",                                           \
              message, (expected), (actual)),                                  \
          1)                                                                   \
       : (printf("\033[38;5;196m[\033[91m\033[1m\033[4mFAILED\033[38;5;196m]"  \
                 "\033[0m:"                                                    \
                 "\033["                                                       \
                 "91m `%s`: "                                                  \
                 "Expected: %d, "                                              \
                 "Actual: %d\n\033[0m",                                        \
                 message, (expected), (actual)),                               \
          0))

#define ASSERT_MULTIPLE(test1, test2) ((test1) && (test2))
#endif

const long int max_instructions = 1000;

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
    uint32_t  opcode              = instruction & 0x7f;
    uint32_t  pc_before           = pc;
    void*     decoded_instruction = decode_functions[opcode](instruction);
    payload_t payload             = {registers, &pc};

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
  int32_t        instr_num = test_data.instr_num;

  switch (instr_num) {
    case 0: {
      int32_t expected = 0x1000 << 12;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "lui a0, 0x1000");
    }
    case 1: {
      int32_t expected = x_before[REG_SP] + x_before[REG_A0];
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "mv	sp,a0");
    }
    case 2: {
      int32_t expected = 0x2000 << 12;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "lui a0, 0x2000");
    }
    case 3: {
      int32_t expected = pc_before + (x_before[REG_RA] << 12);
      return ASSERT_EQUAL_INT(expected, x_after[REG_RA], "auipc ra,0x0");
    }
    case 4: {
      int32_t expected_ra = pc_before + 4;
      int32_t result1 =
          ASSERT_EQUAL_INT(expected_ra, x_after[REG_RA], "(1) jalr	904(ra)");
      int32_t expected_pc = x_before[REG_RA] + 904;
      int32_t result2 =
          ASSERT_EQUAL_INT(expected_pc, pc_after, "(2) jalr	904(ra)");
      return ASSERT_MULTIPLE(result1, result2);
    }
    case 5: {
      int32_t expected = x_before[REG_SP] - 32;
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "addi	sp,sp,-32");
    }
    case 6: {
      int32_t address  = x_before[REG_SP] + 28;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_S0], "sw	s0,28(sp)");
    }
    case 7: {
      int32_t expected = x_before[REG_SP] + 32;
      return ASSERT_EQUAL_INT(expected, x_after[REG_S0], "addi	s0,sp,32");
    }
    case 8: {
      int32_t address  = x_before[REG_S0] - 20;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "sw	a0,-20(s0)");
    }
    case 9: {
      int32_t expected = 0x11 << 12;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "lui	a5,0x11");
    }
    case 10: {
      int32_t address  = x_before[REG_A5] + 4;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_ZERO], "sw	zero,4(a5)");
    }
    case 11: {
      int32_t expected = 0x11 << 12;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "lui	a5,0x11");
    }
    case 12: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_S0] - 20;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A4], "lw	a4,-20(s0)");
    }
    case 13: {
      int32_t address  = x_before[REG_A5];
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_A4], "sw	a4,0(a5)");
    }
    case 14: {
      return ASSERT_EQUAL_INT(0, 0, "nop");
    }
    case 15: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_SP] + 28;
      return ASSERT_EQUAL_INT(expected, x_after[REG_S0], "lw	s0,28(sp)");
    }
    case 16: {
      int32_t expected = x_before[REG_SP] + 32;
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "addi	sp,sp,32");
    }
    case 17: {
      int32_t expected = 0x10210;
      int32_t result1  = ASSERT_EQUAL_INT(expected, pc_after, "(1) ret");
      int32_t result2  = ASSERT_EQUAL_INT(expected, x_after[REG_RA], "(2) ret");
      return ASSERT_MULTIPLE(result1, result2);
    }
    case 18: {
      int32_t expected = 0x1000 << 12;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "lui	a0,0x1000");
    }
    case 19: {
      int32_t expected = x_after[REG_RA] + (0x0 << 12);
      return ASSERT_EQUAL_INT(expected, x_after[REG_RA], "auipc ra,0x0");
    }
    case 20: {
      int32_t expected_ra = pc_before + 4;
      return ASSERT_EQUAL_INT(expected_ra, x_after[REG_RA], "jalr	16(ra)");
    }
    case 21: {
      int32_t expected = x_before[REG_SP] - 48;
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "addi	sp,sp,-48");
    }
    case 22: {
      int32_t address  = x_before[REG_SP] + 44;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_RA], "sw	ra,44(sp)");
    }
    case 23: {
      int32_t address  = x_before[REG_SP] + 40;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_S0], "sw	s0,40(sp)");
    }
    case 24: {
      int32_t expected = x_before[REG_SP] + 48;
      return ASSERT_EQUAL_INT(expected, x_after[REG_S0], "addi	s0,sp,48");
    }
    case 25: {
      int32_t address  = x_before[REG_S0] - 36;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "sw	a0,-36(s0)");
    }
    case 26: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_S0] - 36;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "lw	a5,-36(s0)");
    }
    case 27: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_A5];
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "lw	a5,0(a5)");
    }
    case 28: {
      int32_t address  = x_before[REG_S0] - 20;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "sw	a5,-20(s0)");
    }
    case 29: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_A5] - 36;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "lw	a5,-36(s0)");
    }
    case 30: {
      int32_t expected = x_before[REG_A5] + 4;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "addi	a5,a5,4");
    }
    case 31: {
      int32_t address  = x_before[REG_S0] - 24;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_A5], "sw	a5,-24(s0)");
    }
    case 32: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_S0] - 24;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A1], "lw	a1,-24(s0)");
    }
    case 33: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_S0] - 20;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "lw	a0,-20(s0)");
    }
    case 34: {
      int32_t expected = x_after[REG_RA] + (0x0 << 12);
      return ASSERT_EQUAL_INT(expected, x_after[REG_RA], "auipc	ra,0x0");
    }
    case 35: {
      int32_t expected_ra = pc_before + 4;
      int32_t result1 =
          ASSERT_EQUAL_INT(expected_ra, x_after[REG_RA], "(1) jalr	-332(ra)");
      int32_t expected_pc = x_before[REG_RA] - 332;
      int32_t result2 =
          ASSERT_EQUAL_INT(expected_pc, pc_after, "(2) jalr	-332(ra)");
      return ASSERT_MULTIPLE(result1, result2);
    }
    case 36: {
      int32_t expected = x_before[REG_SP] - 64;
      return ASSERT_EQUAL_INT(expected, x_after[REG_SP], "addi	sp,sp,-64");
    }
    case 37: {
      int32_t address  = x_before[REG_SP] + 60;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_RA], "sw	ra,60(sp)");
    }
    case 38: {
      int32_t address  = x_before[REG_SP] + 56;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_S0], "sw	s0,56(sp)");
    }
    case 39: {
      int32_t expected = x_before[REG_SP] + 64;
      return ASSERT_EQUAL_INT(expected, x_after[REG_S0], "addi	s0,sp,64");
    }
    case 40: {
      int32_t address  = x_before[REG_S0] - 52;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_A0], "sw	a0,-52(s0)");
    }
    case 41: {
      int32_t address  = x_before[REG_S0] - 56;
      int32_t expected = memory_rd_w(mem, address);
      return ASSERT_EQUAL_INT(expected, x_after[REG_A1], "sw	a1,-56(s0)");
    }
    case 42: { // !!!!!!!!!!!!!!!
      int32_t expected = x_before[REG_S0] - 52;
      return ASSERT_EQUAL_INT(expected, x_after[REG_A4], "lw	a4,-52(s0))");
    }
  } // LI, BLT
  return 0;
}
#endif
