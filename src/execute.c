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
  int32_t*            x    = payload->x;
  int32_t             rd      = decoded.rd;

  switch (decoded.funct3) {
    case FUNCT3_ADD_SUB_MUL:
      if (decoded.funct7 == FUNCT7_ADD) {
        x[rd] = x[decoded.rs1] + x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_SUB) {
        x[rd] = x[decoded.rs1] - x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_MUL) {
        x[rd] = x[decoded.rs1] * x[decoded.rs2];
      }
      break;
    case FUNCT3_XOR_DIV:
      if (decoded.funct7 == FUNCT7_XOR) {
        x[rd] = x[decoded.rs1] ^ x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_DIV) {
        x[rd] = x[decoded.rs1] / x[decoded.rs2];
      }
      break;
    case FUNCT3_OR_REM:
      if (decoded.funct7 == FUNCT7_OR) {
        x[rd] = x[decoded.rs1] | x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_REM) {
        x[rd] = x[decoded.rs1] % x[decoded.rs2];
      }
      break;
    case FUNCT3_AND_REMU:
      if (decoded.funct7 == FUNCT7_AND) {
        x[rd] = x[decoded.rs1] & x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_REMU) {
        x[rd] = (uint32_t)x[decoded.rs1] % (uint32_t)x[decoded.rs2];
      }
      break;
    case FUNCT3_SLL_MULH:
      if (decoded.funct7 == FUNCT7_SLL) {
        x[rd] = x[decoded.rs1] << x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_MULH) {
        int64_t result = (int64_t)x[decoded.rs1] * (int64_t)x[decoded.rs2];
        x[rd] = (int32_t)(result >> 32); // upper 32 bits of the int64_t
      }
      break;
    case FUNCT3_SRL_SRA_DIVU:
      if (decoded.funct7 == FUNCT7_SRL) {
        x[rd] = (uint32_t)x[decoded.rs1] >> x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_SRA) {
        x[rd] = x[decoded.rs1] >> x[decoded.rs2];
      } else if (decoded.funct7 == FUNCT7_DIVU) {
        x[rd] = (uint32_t)x[decoded.rs1] / (uint32_t)x[decoded.rs2];
      }
      break;
    case FUNCT3_SLT_MULSU:
      if (decoded.funct7 == FUNCT7_SLT) {
        x[rd] = x[decoded.rs1] < x[decoded.rs2] ? 1 : 0;
      } else if (decoded.funct7 == FUNCT7_MULSU) {
        int64_t  signed_rs1   = (int32_t)x[decoded.rs1];
        uint64_t unsigned_rs2 = (uint32_t)x[decoded.rs2];
        uint64_t result       = signed_rs1 * unsigned_rs2;
        x[rd]              = (int32_t)(result >> 32);
      }
      break;
    case FUNCT3_SLTU_MULU:
      if (decoded.funct7 == FUNCT7_SLTU) {
        x[rd] = (uint32_t)x[decoded.rs1] < (uint32_t)x[decoded.rs2] ? 1 : 0;
      } else if (decoded.funct7 == FUNCT7_MULU) {
        uint64_t unsigned_rs1 = (uint32_t)x[decoded.rs1];
        uint64_t unsigned_rs2 = (uint32_t)x[decoded.rs2];
        uint64_t result       = unsigned_rs1 * unsigned_rs2;
        x[rd]              = (int32_t)(result >> 32);
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
  int32_t*            x    = payload->x;

  if (decoded.opcode == I_TYPE_OPCODE_JALR) {
    //printf("decoded.rs1 = %x\n", decoded.rs1);
    //printf("decoded.rd = %x\n", decoded.rd);
    int32_t rs1 = x[decoded.rs1];
    x[decoded.rd] = *pc + 4;
    *pc      = (rs1 + decoded.imm) & ~1;
    free(instr);
    return;
  }

  if (decoded.opcode == I_TYPE_OPCODE_LOAD) {
    switch (decoded.funct3) {
      case FUNCT3_LB:
        x[decoded.rd] = memory_rd_b(mem, x[decoded.rs1] + decoded.imm);
        break;
      case FUNCT3_LH:
        x[decoded.rd] = memory_rd_h(mem, x[decoded.rs1] + decoded.imm);
        // x[rd]       = ((int32_t)(value1 << 16)) >> 16;
        break;
      case FUNCT3_LW:
        x[decoded.rd] = memory_rd_w(mem, x[decoded.rs1] + decoded.imm);
        // x[rd]       = value2;
        break;
      case FUNCT3_LBU:
        x[decoded.rd] = (unsigned)memory_rd_b(mem, x[decoded.rs1] + decoded.imm);
        // x[rd]       = (int32_t)value3;
        break;
      case FUNCT3_LHU:
        x[decoded.rd] = (unsigned)memory_rd_h(mem, x[decoded.rs1] + decoded.imm);
        // x[rd]        = (int32_t)value4;
        break;
      default:
        printf("Error: Unknown I-type-load instruction\n");
        break;
    }
    free(instr);
    return;
  }

  if (decoded.opcode == I_TYPE_OPCODE_ECALL) {
    printf("=============== ECALL ===============\n");
    switch (x[REG_A7]) {
      case 1: // print integer
        printf("%d", x[REG_A0]);
        break;
      case 2: // print float
        printf("%f", x[REG_A0]);
        break;
      case 3:
        exit(0);
        break;
      case 4: // print string
        char* str = memory_rd_str(mem, x[REG_A0]);
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
        printf("Error: Unknown system call ID %x\n", x[REG_A7]);
        break;
    }
    printf("=====================================\n");
    free(instr);
    return;
  }

  switch (decoded.funct3) {
    case FUNCT3_ADDI:
      x[decoded.rd] = x[decoded.rs1] + decoded.imm;
      break;
    case FUNCT3_XORI:
      x[decoded.rd] = x[decoded.rs1] ^ decoded.imm;
      break;
    case FUNCT3_ORI:
      x[decoded.rd] =x[decoded.rs1] | decoded.imm;
      break;
    case FUNCT3_ANDI:
      x[decoded.rd] = x[decoded.rs1] & decoded.imm;
      break;
    case FUNCT3_SLLI:
      x[decoded.rd] = x[decoded.rs1] << (decoded.imm & 0x1F);
      break;
    case FUNCT3_SRLI_SRAI:
      if (decoded.imm >> 10 == FUNCT7_SRLI) {
        x[decoded.rd] = x[decoded.rs1] >> (decoded.imm & 0x1F);
      } else if (decoded.imm >> 10 == FUNCT7_SRAI) {
        // TODO: MSB EXTEND
        x[decoded.rd] = x[decoded.rs1] >> (decoded.imm & 0x1F);
      }
      break;
    case FUNCT3_SLTI:
      x[decoded.rd] = x[decoded.rs1] < decoded.imm ? 1 : 0;
      break;
    case FUNCT3_SLTIU:
      // TODO: zero extend
      uint32_t unsigned_rs1 = (uint32_t)x[decoded.rs1];
      uint32_t unsigned_imm = (uint32_t)decoded.imm;
      x[decoded.rd]              = unsigned_rs1 < unsigned_imm ? 1 : 0;
      break;
    default:
      printf("Error: Unknown I-type instruction\n");
      break;
  }
  free(instr);
}

void execute_S_type(void* instr, struct memory* mem, payload_t* payload) {
  stype_instruction_t decoded = *(stype_instruction_t*)instr;
  int32_t*            x    = payload->x;
  int32_t             rs1     = x[decoded.rs1];
  int32_t             rs2     = x[decoded.rs2];
  int32_t             imm     = decoded.imm;
  int32_t             address = rs1 + imm;

  switch (decoded.funct3) {
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
  int32_t*            x    = payload->x;

  switch (decoded.funct3) {
    case FUNCT3_BEQ:
      if (x[decoded.rs1] == x[decoded.rs2]) {
        *pc += decoded.imm;
      }
      break;
    case FUNCT3_BNE:
      if (x[decoded.rs1] != x[decoded.rs2]) {
        *pc += decoded.imm;
      }
      break;
    case FUNCT3_BLT:
      if (x[decoded.rs1] < x[decoded.rs2]) {
        *pc += decoded.imm;
      }
      break;
    case FUNCT3_BGE:
      if (x[decoded.rs1] >= x[decoded.rs2]) {
        *pc += decoded.imm;
      }
      break;
    case FUNCT3_BLTU:
      if (x[decoded.rs1] < (uint32_t)x[decoded.rs2]) {
        *pc += decoded.imm;
      }
      break;
    case FUNCT3_BGEU:
      if (x[decoded.rs1] >= (uint32_t)x[decoded.rs2]) {
        *pc += decoded.imm;
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
  int32_t*            x    = payload->x;

  switch (decoded.opcode) {
    case JAL_OPCODE:
      x[decoded.rd] = *pc + 4;
      *pc += decoded.imm;
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
  int32_t*            x    = payload->x;

  switch (decoded.opcode) {
    case LUI_OPCODE:
      x[decoded.rd] = (uint32_t)decoded.imm << 12;
      break;
    case AUIPC_OPCODE:
      x[decoded.rd] = *pc + (decoded.imm << 12);
      break;
    default:
      printf("Error: Unknown U-type instruction\n");
      break;
  }
  free(instr);
}
