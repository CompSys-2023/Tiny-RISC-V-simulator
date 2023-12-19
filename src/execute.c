#include "execute.h"
#include "memory.h"
#include "simulate.h"

void initialize_execute_functions(exec_fn_ptr* functions) {
  functions[R_TYPE_OPCODE]       = &execute_R_type;
  functions[I_TYPE_OPCODE]       = &execute_I_type;
  functions[I_TYPE_OPCODE_ECALL] = &execute_I_type;
  functions[I_TYPE_OPCODE_LOAD]  = &execute_I_type;
  functions[I_TYPE_OPCODE_JALR]  = &execute_I_type;
  functions[S_TYPE_OPCODE]       = &execute_S_type;
  functions[B_TYPE_OPCODE]       = &execute_B_type;
  functions[J_TYPE_OPCODE]       = &execute_J_type;
  functions[U_TYPE_OPCODE_LUI]   = &execute_U_type;
  functions[U_TYPE_OPCODE_AUI]   = &execute_U_type;
}

void execute_R_type(void* instr, struct memory* mem, payload_t* payload) {
  rtype_instruction_t decoded = *(rtype_instruction_t*)instr;
  int32_t*            regs    = payload->regs;
  uint32_t            rd      = decoded.rd;
  uint32_t            funct3  = decoded.funct3;
  int32_t             rs1     = regs[decoded.rs1];
  int32_t             rs2     = regs[decoded.rs2];
  uint32_t            funct7  = decoded.funct7;

  switch (funct3) {
    case FUNCT3_ADD_SUB_MUL:
      if (funct7 == FUNCT7_ADD) {
        regs[rd] = rs1 + rs2;
      } else if (funct7 == FUNCT7_SUB) {
        regs[rd] = rs1 - rs2;
      } else if (funct7 == FUNCT7_MUL) {
        regs[rd] = rs1 * rs2;
      }
      break;
    case FUNCT3_XOR_DIV:
      if (funct7 == FUNCT7_XOR) {
        regs[rd] = rs1 ^ rs2;
      } else if (funct7 == FUNCT7_DIV) {
        regs[rd] = rs1 / rs2;
      }

      break;
    case FUNCT3_OR_REM:
      if (funct7 == FUNCT7_OR) {
        regs[rd] = rs1 | rs2;
      } else if (funct7 == FUNCT7_REM) {
        regs[rd] = rs1 % rs2;
      }
      break;
    case FUNCT3_AND_REMU:
      if (funct7 == FUNCT7_AND) {
        regs[rd] = rs1 & rs2;
      } else if (funct7 == FUNCT7_REMU) {
        regs[rd] = (uint32_t)rs1 % (uint32_t)rs2;
      }
      break;
    case FUNCT3_SLL_MULH:
      if (funct7 == FUNCT7_SLL) {
        regs[rd] = (uint32_t)rs1 << (uint32_t)rs2;
      } else if (funct7 == FUNCT7_MULH) {
        int64_t result = (int64_t)rs1 * (int64_t)rs2;
        regs[rd]       = (int32_t)(result >> 32);
      }
      break;
    case FUNCT3_SRL_SRA_DIVU:
      if (funct7 == FUNCT7_SRL) {
        regs[rd] = (uint32_t)rs1 >> (uint32_t)rs2;
      } else if (funct7 == FUNCT7_SRA) {
        regs[rd] = rs1 >> rs2;
      } else if (funct7 == FUNCT7_DIVU) {
        regs[rd] = (uint32_t)rs1 / (uint32_t)rs2;
      }
      break;
    case FUNCT3_SLT_MULSU:
      if (funct7 == FUNCT7_SLT) {
        regs[rd] = rs1 < rs2 ? 1 : 0;
      } else if (funct7 == FUNCT7_MULSU) {
        int64_t  signed_rs1   = (int32_t)rs1;
        uint64_t unsigned_rs2 = (uint32_t)rs2;
        uint64_t result       = signed_rs1 * unsigned_rs2;
        regs[rd]              = (int32_t)(result >> 32);
      }
      break;
    case FUNCT3_SLTU_MULU:
      if (funct7 == FUNCT7_SLTU) {
        regs[rd] = (uint32_t)rs1 < (uint32_t)rs2 ? 1 : 0;
      } else if (funct7 == FUNCT7_MULU) {
        uint64_t unsigned_rs1 = (uint32_t)rs1;
        uint64_t unsigned_rs2 = (uint32_t)rs2;
        uint64_t result       = unsigned_rs1 * unsigned_rs2;
        regs[rd]              = (int32_t)(result >> 32);
      }
      break;
    default:
      printf("Error: Unknown R-type instruction\n");
      break;
  }
  free(instr);
}

void execute_I_type(void* instr, struct memory* mem, payload_t* payload) {
  itype_instruction_t decoded = *(itype_instruction_t*)instr;
  int32_t*            regs    = payload->regs;
  int32_t             imm     = decoded.imm;
  uint32_t*           pc      = payload->pc;
  uint32_t            opcode  = decoded.opcode;
  uint32_t            rd      = decoded.rd;
  uint32_t            funct3  = decoded.funct3;
  int32_t             rs1     = regs[decoded.rs1];

  if (opcode == I_TYPE_OPCODE_JALR) {
    regs[rd] = *pc + 4;
    *pc      = (rs1 + imm) & ~1;
    free(instr);
    return;
  }

  if (opcode == I_TYPE_OPCODE_LOAD) {
    switch (funct3) {
      case FUNCT3_LB: {
        regs[rd] = memory_rd_b(mem, rs1 + imm);
        break;
      }
      case FUNCT3_LH: {
        regs[rd] = memory_rd_h(mem, rs1 + imm);
        break;
      }
      case FUNCT3_LW:
        regs[rd] = memory_rd_w(mem, rs1 + imm);
        break;
      case FUNCT3_LBU: {
        regs[rd] = (unsigned)memory_rd_b(mem, rs1 + imm);
        break;
      }
      case FUNCT3_LHU: {
        regs[rd] = (unsigned)memory_rd_h(mem, rs1 + imm);
        break;
      }
      default:
        printf("Error: Unknown I-type-load instruction\n");
        break;
    }
    free(instr);
    return;
  }

  if (opcode == I_TYPE_OPCODE_ECALL) {
    printf("=====================================\n");
    switch (regs[REG_A7]) {
      case 1:
        regs[REG_A0] = getchar();
        break;
      case 2:
        putchar(regs[REG_A0]);
        fflush(stdout);
        break;
      case 3:
      case 93:
        printf("Exit code: %d\n", regs[REG_A0]);
        exit(regs[REG_A0]);
      default:
        printf("Error: Unknown system call ID.. %05x\n", regs[REG_A7]);
        break;
    }
    printf("=====================================\n");
    free(instr);
    return;
  }

  switch (funct3) {
    case FUNCT3_ADDI:
      regs[rd] = rs1 + imm;
      break;
    case FUNCT3_XORI:
      regs[rd] = rs1 ^ imm;
      break;
    case FUNCT3_ORI:
      regs[rd] = rs1 | imm;
      break;
    case FUNCT3_ANDI:
      regs[rd] = rs1 & imm;
      break;
    case FUNCT3_SLLI:
      regs[rd] = (unsigned int)rs1 << (imm & 0x1F);
      break;
    case FUNCT3_SRLI_SRAI:
      if (imm >> 10 == FUNCT7_SRLI) {
        regs[rd] = (unsigned int)rs1 >> (imm & 0x1F);
      } else if (imm >> 10 == FUNCT7_SRAI) {
        regs[rd] = (signed int)rs1 >> (imm & 0x1F);
      }
      break;
    case FUNCT3_SLTI:
      regs[rd] = rs1 < imm ? 1 : 0;
      break;
    case FUNCT3_SLTIU:
      uint32_t unsigned_rs1 = (uint32_t)rs1;
      uint32_t unsigned_imm = (uint32_t)imm;
      regs[rd]              = unsigned_rs1 < unsigned_imm ? 1 : 0;
      break;
    default:
      printf("Error: Unknown I-type instruction\n");
      break;
  }
  free(instr);
}

void execute_S_type(void* instr, struct memory* mem, payload_t* payload) {
  stype_instruction_t decoded = *(stype_instruction_t*)instr;
  int32_t*            regs    = payload->regs;
  uint32_t            opcode  = decoded.opcode;
  uint32_t            funct3  = decoded.funct3;
  int32_t             rs1     = regs[decoded.rs1];
  int32_t             rs2     = regs[decoded.rs2];
  int32_t             imm     = decoded.imm;
  int                 addr    = rs1 + imm;
  switch (funct3) {
    case FUNCT3_SB:
      memory_wr_b(mem, addr, (int8_t)rs2);
      break;
    case FUNCT3_SH:
      memory_wr_h(mem, addr, (int16_t)rs2);
      break;
    case FUNCT3_SW:
      memory_wr_w(mem, addr, (int32_t)rs2);
      break;
    default:
      printf("Error: Unknown S-type instruction\n");
      break;
  }
  free(instr);
}

int  beq(int32_t rs1, int32_t rs2) { return rs1 == rs2; }
int  bne(int32_t rs1, int32_t rs2) { return rs1 != rs2; }
int  blt(int32_t rs1, int32_t rs2) { return rs1 < rs2; }
int  bge(int32_t rs1, int32_t rs2) { return rs1 >= rs2; }
int  bltu(int32_t rs1, int32_t rs2) { return (uint32_t)rs1 < (uint32_t)rs2; }
int  bgeu(int32_t rs1, int32_t rs2) { return (uint32_t)rs1 >= (uint32_t)rs2; }
void execute_B_type(void* instr, struct memory* mem, payload_t* payload) {
  static branch_func_t branch_funcs[] = {beq, bne, NULL, NULL,
                                         blt, bge, bltu, bgeu};
  btype_instruction_t  decoded        = *(btype_instruction_t*)instr;
  int32_t*             regs           = payload->regs;
  int32_t              imm            = decoded.imm << 1;
  uint32_t*            pc             = payload->pc;
  uint32_t             opcode         = decoded.opcode;
  uint32_t             funct3         = decoded.funct3;
  uint32_t             rs1            = regs[decoded.rs1];
  uint32_t             rs2            = regs[decoded.rs2];
  int                  branch_addr    = *pc + imm;

  if (branch_funcs[funct3](rs1, rs2)) {
    *pc = branch_addr;
  }

  free(instr);
}

void execute_J_type(void* instr, struct memory* mem, payload_t* payload) {
  jtype_instruction_t decoded = *(jtype_instruction_t*)instr;
  uint32_t            opcode  = decoded.opcode;
  uint32_t            rd      = decoded.rd;
  uint32_t*           pc      = payload->pc;
  int32_t*            regs    = payload->regs;
  int32_t             imm     = decoded.imm << 1;

  switch (opcode) {
    case JAL_OPCODE:
      regs[rd] = *pc + 4;
      *pc += imm;
      break;
    default:
      printf("Error: Unknown J-type instruction\n");
      break;
  }
  free(instr);
}

void execute_U_type(void* instr, struct memory* mem, payload_t* payload) {
  utype_instruction_t decoded = *(utype_instruction_t*)instr;
  uint32_t            opcode  = decoded.opcode;
  uint32_t            rd      = decoded.rd;
  uint32_t*           pc      = payload->pc;
  int32_t*            regs    = payload->regs;
  int32_t             imm     = decoded.imm;

  switch (opcode) {
    case LUI_OPCODE:
      regs[rd] = (imm << 12);
      break;
    case AUIPC_OPCODE:
      regs[rd] = *pc + (imm << 12);
      break;
    default:
      printf("Error: Unknown U-type instruction\n");
      break;
  }
  free(instr);
}
