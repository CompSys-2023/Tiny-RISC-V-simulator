#include "execute.h"
#include "memory.h"
#include "simulate.h"

void initialize_execute_functions(exec_fn_ptr* functions) {
  functions[R_TYPE_OPCODE]     = &execute_R_type;
  functions[I_TYPE_OPCODE]     = &execute_I_type;
  functions[S_TYPE_OPCODE]     = &execute_S_type;
  functions[B_TYPE_OPCODE]     = &execute_B_type;
  functions[U_TYPE_OPCODE_LUI] = &execute_U_type;
  functions[U_TYPE_OPCODE_AUI] = &execute_U_type;
  functions[J_TYPE_OPCODE]     = &execute_J_type;
}

void execute_R_type(void* instruction, struct memory* mem, payload_t* payload) {
  rtype_instruction_t decoded = *(rtype_instruction_t*)instruction;
  int*                regs    = payload->regs;
  int                 rs1     = regs[decoded.rs1];
  int                 rs2     = regs[decoded.rs2];

  switch (decoded.funct3) {
    case ADD_FUNCT3:
      if (decoded.funct7 == ADD_FUNCT7) {
        regs[decoded.rd] = rs1 + rs2;
      } else if (decoded.funct7 == SUB_FUNCT7) {
        regs[decoded.rd] = rs1 - rs2;
      }
      break;
    case SLL_FUNCT3:
      regs[decoded.rd] = rs1 << rs2;
      break;
    case SLT_FUNCT3:
      regs[decoded.rd] = rs1 < rs2;
      break;
    case SLTU_FUNCT3:
      regs[decoded.rd] = (uint32_t)rs1 < (uint32_t)rs2;
      break;
    case XOR_FUNCT3:
      regs[decoded.rd] = rs1 ^ rs2;
      break;
    case SRL_SRA_FUNCT3:
      if (decoded.funct7 == SRL_FUNCT7) {
        regs[decoded.rd] = (uint32_t)rs1 >> (uint32_t)rs2;
      } else if (decoded.funct7 == SRA_FUNCT7) {
        regs[decoded.rd] = rs1 >> rs2;
      }
      break;
    case OR_FUNCT3:
      regs[decoded.rd] = rs1 | rs2;
      break;
    case AND_FUNCT3:
      regs[decoded.rd] = rs1 & rs2;
      break;
  }
}

void execute_I_type(void* instr, struct memory* mem, payload_t* payload) {
  itype_instruction_t decoded = *(itype_instruction_t*)instr;
  int*                regs    = payload->regs;
  int*                pc      = payload->pc;
  int                 rs1     = regs[decoded.rs1];

  switch (decoded.funct3) {
    case JALR_ADDI_FUNCT3:
      break;
    case SLTI_FUNCT3:
      regs[decoded.rd] = (uint32_t)rs1 < regs[decoded.imm];
      break;
    case SLTIU_FUNCT3:
      regs[decoded.rd] = (uint32_t)rs1 < regs[decoded.imm];
      break;
    case XORI_FUNCT3:
      regs[decoded.rd] = (uint32_t)rs1 ^ regs[decoded.imm];
      break;
    case ORI_FUNCT3:
      regs[decoded.rd] = (uint32_t)rs1 | regs[decoded.imm];
      break;
    case ANDI_FUNCT3:
      regs[decoded.rd] = (uint32_t)rs1 & regs[decoded.imm];
      break;
    default:
      break;
  }
}

void execute_S_type(void* instr, struct memory* mem, payload_t* payload) {
  stype_instruction_t decoded = *(stype_instruction_t*)instr;
  int*                regs    = payload->regs;
  uint32_t            offset  = (decoded.imm_11_5 << 5) | decoded.imm_4_0;
  uint32_t            address = regs[decoded.rs1] + offset;

  void (*mem_write_ptr)(struct memory*, int, int);

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
}

void execute_B_type(void* instr, struct memory* mem, payload_t* payload) {
  btype_instruction_t decoded = *(btype_instruction_t*)instr;
  int*                regs    = payload->regs;
  int*                pc      = payload->pc;
  int                 rs1     = regs[decoded.rs1];
  int                 rs2     = regs[decoded.rs2];

  switch (decoded.funct3) {
    case BEQ_FUNCT3:
      if (rs1 == rs2) {
        int32_t offset = (decoded.imm_12 << 12) | (decoded.imm_11 << 11) |
                         (decoded.imm_10_5 << 5) | (decoded.imm_4_1 << 1);

        *pc += offset;
      }
      break;
  }
}

void execute_U_type(void* instr, struct memory* mem, payload_t* payload) {
  utype_instruction_t decoded = *(utype_instruction_t*)instr;
  int*                regs    = payload->regs;
  int*                pc      = payload->pc;

  switch (decoded.opcode) {
    case LUI_OPCODE:
      regs[decoded.rd] = decoded.imm << 12;
      break;
    case AUIPC_OPCODE:
      regs[decoded.rd] = *pc + (decoded.imm << 12);
      break;
    default:
      break;
  }
}

void execute_J_type(void* instruction, struct memory* mem, payload_t* payload) {
  jtype_instruction_t decoded = *(jtype_instruction_t*)instruction;
  int*                regs    = payload->regs;
  int*                pc      = payload->pc;

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
}
