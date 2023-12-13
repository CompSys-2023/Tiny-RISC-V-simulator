#include "execute.h"
#include "memory.h"
#include "simulate.h"

void initialize_execute_functions(exec_fn_ptr* functions) {
  functions[R_TYPE_OPCODE]      = &execute_R_type;
  functions[I_TYPE_OPCODE]      = &execute_I_type;
  functions[I_TYPE_OPCODE_LOAD] = &execute_I_type;
  functions[I_TYPE_OPCODE_JALR] = &execute_I_type;
  functions[S_TYPE_OPCODE]      = &execute_S_type;
  functions[B_TYPE_OPCODE]      = &execute_B_type;
  functions[U_TYPE_OPCODE_LUI]  = &execute_U_type;
  functions[U_TYPE_OPCODE_AUI]  = &execute_U_type;
  functions[J_TYPE_OPCODE]      = &execute_J_type;
}

void execute_R_type(void* instr, struct memory* mem, payload_t* payload) {
  printf("Executing R:\n");
  if (instr == NULL) {
    printf("Null instruction pointer.\n");
    return;
  }
  rtype_instruction_t decoded = *(rtype_instruction_t*)instr;
  uint32_t*           regs    = payload->regs;
  uint32_t            rs1     = regs[decoded.rs1];
  uint32_t            rs2     = regs[decoded.rs2];

  switch (decoded.funct3) {
    case ADD_SUB_MUL_FUNCT3:
      if (decoded.funct7 == ADD_FUNCT7) {
        regs[decoded.rd] = rs1 + rs2;
      } else if (decoded.funct7 == SUB_FUNCT7) {
        regs[decoded.rd] = rs1 - rs2;
      } else if (decoded.funct7 == MUL_FUNCT7) {
        regs[decoded.rd] = rs1 * rs2;
      }
      break;

    case SLL_MULH_FUNCT3:
      if (decoded.funct7 == SLL_FUNCT7) {
        uint32_t shamt   = rs2 & 0x1f;
        regs[decoded.rd] = rs1 << shamt;
      } else if (decoded.funct7 == MULH_FUNCT7) {
        regs[decoded.rd] = ((int32_t)rs1) * ((int32_t)rs2) >> 32;
      }
      break;
    case SLT_MULHSU_FUNCT3:
      if (decoded.funct7 == SLT_FUNCT7) {
        regs[decoded.rd] = ((int32_t)rs1) < ((int32_t)rs2);
      } else if (decoded.funct7 == MULHSU_FUNCT7) {
        regs[decoded.rd] = ((int32_t)rs1) * ((uint32_t)rs2) >> 32;
      }
      break;
    case SLTU_FUNCT3:
      regs[decoded.rd] = (uint32_t)rs1 < (uint32_t)rs2;
      break;
    case XOR_DIV_FUNCT3:
      if (decoded.funct7 == XOR_FUNCT7) {
        regs[decoded.rd] = rs1 ^ rs2;
      } else if (decoded.funct7 == DIV_FUNCT7) {
        regs[decoded.rd] = rs1 / rs2;
      }
      break;
    case SRL_SRA_DIVU_FUNCT3:
      if (decoded.funct7 == SRL_FUNCT7) {
        regs[decoded.rd] = (uint32_t)rs1 >> (uint32_t)rs2;
      } else if (decoded.funct7 == SRA_FUNCT7) {
        regs[decoded.rd] = rs1 >> rs2;
      } else if (decoded.funct7 == DIVU_FUNCT7) {
        regs[decoded.rd] = (uint32_t)rs1 / (uint32_t)rs2;
      }
      break;
    case OR_REM_FUNCT3:
      if (decoded.funct7 == OR_FUNCT7) {
        regs[decoded.rd] = rs1 | rs2;
      } else if (decoded.funct7 == REM_FUNCT7) {
        regs[decoded.rd] = rs1 % rs2;
      }
      break;
    case AND_REMU_FUNCT3:
      if (decoded.funct7 == AND_FUNCT7) {
        regs[decoded.rd] = rs1 & rs2;
      } else if (decoded.funct7 == REM_FUNCT7) {
        regs[decoded.rd] = rs1 % rs2;
      }
      break;
  }
  printf("R: Completed\n");
}

void execute_I_type(void* instr, struct memory* mem, payload_t* payload) {
  printf("Executing I:\n");
  if (instr == NULL) {
    printf("Null instruction pointer.\n");
    return;
  }

  itype_instruction_t decoded = *(itype_instruction_t*)instr;
  uint32_t*           regs    = payload->regs;
  uint32_t*           pc      = payload->pc;
  uint32_t            rs1     = regs[decoded.rs1];

  if (decoded.opcode == JALR_OPCODE) { // outlier for I-type
    regs[decoded.rd] = pc + 4;
    *pc += (decoded.imm << 20) >> 20;
    return;
  }
  printf("I: Completed\n");

  switch (decoded.opcode) {
    case I_TYPE_OPCODE_LOAD:
      switch (decoded.funct3) {
        case LB_FUNCT3:
          regs[decoded.rd] = memory_rd_b(mem, rs1 + decoded.imm);
          break;
        case LH_FUNCT3:
          regs[decoded.rd] = memory_rd_h(mem, rs1 + decoded.imm);
          break;
        case LW_FUNCT3:
          regs[decoded.rd] = memory_rd_w(mem, rs1 + decoded.imm);
          break;
        case LBU_FUNCT3:
          regs[decoded.rd] = memory_rd_b(mem, rs1 + decoded.imm);
          break;
        case LHU_FUNCT3:
          regs[decoded.rd] = memory_rd_h(mem, rs1 + decoded.imm);
          break;
      }
    case I_TYPE_OPCODE:
      switch (decoded.funct3) {
        case ADDI_FUNCT3:
          regs[decoded.rd] = rs1 + decoded.imm;
          break;
        case SLTI_FUNCT3:
          regs[decoded.rd] = rs1 < decoded.imm;
          break;
        case SLTIU_FUNCT3:
          regs[decoded.rd] = (uint32_t)rs1 < (uint32_t)decoded.imm;
          break;
        case XORI_FUNCT3:
          regs[decoded.rd] = rs1 ^ decoded.imm;
          break;
        case ORI_FUNCT3:
          regs[decoded.rd] = rs1 | decoded.imm;
          break;
        case ANDI_FUNCT3:
          regs[decoded.rd] = rs1 & decoded.imm;
          break;
        case SLLI_FUNCT3:
          uint32_t shamt   = decoded.imm & 0x1f;
          regs[decoded.rd] = rs1 << shamt;
          break;
        case SRLI_SRAI_FUNCT3:
          if (decoded.imm >> 5 == SRLI_FUNCT7) {
            uint32_t shamt   = decoded.imm & 0x1f;
            regs[decoded.rd] = (uint32_t)rs1 >> shamt;
          } else if (decoded.imm >> 5 == SRAI_FUNCT7) {
            int32_t shamt    = decoded.imm & 0x1f;
            regs[decoded.rd] = rs1 >> shamt;
          }
      }
      break;
  }
}

void execute_S_type(void* instr, struct memory* mem, payload_t* payload) {
  printf("Executing S:\n");
  if (instr == NULL) {
    printf("Null instruction pointer.\n");
    return;
  }
  stype_instruction_t decoded = *(stype_instruction_t*)instr;
  uint32_t*           regs    = payload->regs;
  uint32_t            offset  = (decoded.imm_11_5 << 5) | decoded.imm_4_0;
  uint32_t            address = regs[decoded.rs1] + offset;

  void (*mem_write_ptr)(struct memory*, int, int) = NULL;

  if (decoded.funct3 == SB_FUNCT3) {
    mem_write_ptr = &memory_wr_b;
  } else if (decoded.funct3 == SH_FUNCT3) {
    mem_write_ptr = &memory_wr_h;
  } else if (decoded.funct3 == SW_FUNCT3) {
    mem_write_ptr = &memory_wr_w;
  }

  if (mem_write_ptr == NULL)
    return;

  mem_write_ptr(mem, address, regs[decoded.rs2]);
  printf("S: Completed\n");
}

void execute_B_type(void* instr, struct memory* mem, payload_t* payload) {
  printf("Executing B:\n");
  if (instr == NULL) {
    printf("Null instruction pointer.\n");
    return;
  }
  btype_instruction_t decoded = *(btype_instruction_t*)instr;
  uint32_t*           regs    = payload->regs;
  uint32_t*           pc      = payload->pc;
  uint32_t            rs1     = regs[decoded.rs1];
  uint32_t            rs2     = regs[decoded.rs2];

  switch (decoded.funct3) {
    case BEQ_FUNCT3:
      if (rs1 == rs2) {
        int32_t offset = (decoded.imm_12 << 12) | (decoded.imm_11 << 11) |
                         (decoded.imm_10_5 << 5) | (decoded.imm_4_1 << 1);

        *pc += offset;
      }
      break;
  }
  printf("B: Completed\n");
  // MANGLER BNE; BLT; BGE; BLTU OG BGEU
}

void execute_U_type(void* instr, struct memory* mem, payload_t* payload) {
  printf("Executing U:\n");
  if (instr == NULL) {
    printf("Null instruction pointer.\n");
    return;
  }
  utype_instruction_t decoded = *(utype_instruction_t*)instr;
  uint32_t*           regs    = payload->regs;
  uint32_t*           pc      = payload->pc;

  switch (decoded.opcode) {
    case LUI_OPCODE:
      regs[decoded.rd] = decoded.imm << 12;
      printf("LUI\n");
      break;
    case AUIPC_OPCODE:
      regs[decoded.rd] = *pc + (decoded.imm << 12);
      printf("AUIPC\n");
      break;
    default:
      break;
  }
  printf("U: Completed\n");
}

void execute_J_type(void* instr, struct memory* mem, payload_t* payload) {
  printf("Executing J:\n");
  if (instr == NULL) {
    printf("Null instruction pointer.\n");
    return;
  }
  jtype_instruction_t decoded = *(jtype_instruction_t*)instr;
  uint32_t*           regs    = payload->regs;
  uint32_t*           pc      = payload->pc;

  switch (decoded.opcode) {
    case JAL_OPCODE:
      regs[decoded.rd] = pc + 4;
      uint32_t offset  = (decoded.imm_20 << 20) | (decoded.imm_19_12 << 12) |
                        (decoded.imm_11 << 11) | (decoded.imm_10_1 << 1);
      *pc += offset;
      break;
    default:
      break;
  }
  printf("J: Completed\n");
}