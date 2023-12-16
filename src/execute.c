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

char* memory_rd_str(struct memory* mem, int addr) {
  char* str = (char*)malloc(1); // Initially allocate memory for one character
  int   length = 0;
  int   value;

  while (1) {
    value       = memory_rd_b(mem, addr + length); // Read a byte from memory
    str         = (char*)realloc(str, length + 2); // Allocate more memory
    str[length] = (char)value; // Store the byte in the string
    length++;

    if (value == '\0') {
      break; // Break if null character is found
    }
  }

  return str;
}

void execute_R_type(void* instr, struct memory* mem, payload_t* payload) {
  rtype_instruction_t decoded = *(rtype_instruction_t*)instr;
  int32_t*            regs    = payload->regs;
  int32_t             opcode  = decoded.opcode;
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
        regs[rd] = rs1 << rs2;
      } else if (funct7 == FUNCT7_MULH) {
        int64_t result = (int64_t)rs1 * (int64_t)rs2;
        regs[rd] = (int32_t)(result >> 32); // upper 32 bits of the int64_t
      }
      break;
    case FUNCT3_SRL_SRA_DIVU:
      if (funct7 == FUNCT7_SRL) {
        regs[rd] = (uint32_t)rs1 >> rs2;
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
  uint32_t*           pc      = payload->pc;
  int32_t*            regs    = payload->regs;
  uint32_t            opcode  = decoded.opcode;
  uint32_t            rd      = decoded.rd;
  uint32_t            funct3  = decoded.funct3;
  int32_t             rs1     = regs[decoded.rs1];
  int32_t             imm     = decoded.imm;

  if (opcode == I_TYPE_OPCODE_JALR) {
    regs[rd] = *pc + 4;
    *pc      = (rs1 + imm) & ~1;
    free(instr);
    return;
  }

  if (opcode == I_TYPE_OPCODE_LOAD) {
    switch (funct3) {
      case FUNCT3_LB:
        regs[rd] = memory_rd_b(mem, rs1 + imm);
        break;
      case FUNCT3_LH:
        regs[rd] = memory_rd_h(mem, rs1 + imm);
        // regs[rd]       = ((int32_t)(value1 << 16)) >> 16;
        break;
      case FUNCT3_LW:
        regs[rd] = memory_rd_w(mem, rs1 + imm);
        // regs[rd]       = value2;
        break;
      case FUNCT3_LBU:
        regs[rd] = (unsigned)memory_rd_b(mem, rs1 + imm);
        // regs[rd]       = (int32_t)value3;
        break;
      case FUNCT3_LHU:
        regs[rd] = (unsigned)memory_rd_h(mem, rs1 + imm);
        // regs[rd]        = (int32_t)value4;
        break;
      default:
        printf("Error: Unknown I-type-load instruction\n");
        break;
    }
    free(instr);
    return;
  }

  if (opcode == I_TYPE_OPCODE_ECALL) {
    printf("=============== ECALL ===============\n");
    switch (regs[REG_A7]) {
      case 1: // print integer
        printf("%d", regs[REG_A0]);
        break;
      case 2: // print float
        printf("%f", regs[REG_A0]);
        break;
      case 4: // print string
        char* str = memory_rd_str(mem, regs[REG_A0]);
        if (str != NULL) {
          printf("%s", str);
        } else {
          printf("Error: Invalid memory address\n");
        }
        break;
      case 10: // exit
        exit(0);
        break;
      default:
        printf("Error: Unknown system call ID %d\n", regs[REG_A7]);
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
      regs[rd] = rs1 << (imm & 0x1F);
      break;
    case FUNCT3_SRLI_SRAI:
      if (imm >> 10 == FUNCT7_SRLI) {
        regs[rd] = rs1 >> (imm & 0x1F);
      } else if (imm >> 10 == FUNCT7_SRAI) {
        // TODO: MSB EXTEND
        regs[rd] = rs1 >> (imm & 0x1F);
      }
      break;
    case FUNCT3_SLTI:
      regs[rd] = rs1 < imm ? 1 : 0;
      break;
    case FUNCT3_SLTIU:
      // TODO: zero extend
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
  int32_t             opcode  = decoded.opcode;
  int32_t             funct3  = decoded.funct3;
  int32_t             rs1     = regs[decoded.rs1];
  int32_t             rs2     = regs[decoded.rs2];
  int32_t             imm     = decoded.imm;
  int32_t             address = rs1 + imm;

  switch (funct3) {
    case FUNCT3_SB:
      memory_wr_b(mem, address, (int8_t)rs2);
      break;
    case FUNCT3_SH:
      memory_wr_h(mem, address, (int16_t)rs2);
      break;
    case FUNCT3_SW:
      memory_wr_w(mem, address, (int32_t)rs2);
      break;
    default:
      printf("Error: Unknown S-type instruction\n");
      break;
  }
  free(instr);
}

void execute_B_type(void* instr, struct memory* mem, payload_t* payload) {
  btype_instruction_t decoded = *(btype_instruction_t*)instr;
  uint32_t*           pc      = payload->pc;
  int32_t*            regs    = payload->regs;
  int32_t             opcode  = decoded.opcode;
  int32_t             funct3  = decoded.funct3;
  int32_t             rs1     = regs[decoded.rs1];
  int32_t             rs2     = regs[decoded.rs2];
  int32_t             imm     = decoded.imm;

  switch (funct3) {
    case FUNCT3_BEQ:
      if (rs1 == rs2) {
        *pc += imm;
      }
      break;
    case FUNCT3_BNE:
      printf("=============\n");
      printf("PC: %05x\n", *pc);
      if (rs1 != rs2) {
        *pc += imm;
        printf("imm: %d\n", imm);
        printf("PC: %05x\n", *pc);
      }
      printf("PC: %05x\n", *pc);
      printf("=============\n");
      break;
    case FUNCT3_BLT:
      if (rs1 < rs2) {
        *pc += imm;
      }
      break;
    case FUNCT3_BGE:
      if (rs1 >= rs2) {
        *pc += imm;
      }
      break;
    case FUNCT3_BLTU:
      if (rs1 < (uint32_t)rs2) {
        *pc += imm;
      }
      break;
    case FUNCT3_BGEU:
      if (rs1 >= (uint32_t)rs2) {
        *pc += imm;
      }
      break;
    default:
      printf("Error: Unknown B-type instruction\n");
      break;
  }
  free(instr);
}

void execute_J_type(void* instr, struct memory* mem, payload_t* payload) {
  jtype_instruction_t decoded = *(jtype_instruction_t*)instr;
  uint32_t*           pc      = payload->pc;
  int32_t*            regs    = payload->regs;
  int32_t             opcode  = decoded.opcode;
  uint32_t            rd      = decoded.rd;
  uint32_t            imm     = decoded.imm;

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
  uint32_t*           pc      = payload->pc;
  int32_t             opcode  = decoded.opcode;
  uint32_t            rd      = decoded.rd;
  int32_t*            regs    = payload->regs;
  int32_t             imm     = decoded.imm;

  switch (opcode) {
    case LUI_OPCODE:
      regs[rd] = (uint32_t)imm << 12;
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
