#include "test.h"
#include "macros.h"

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
