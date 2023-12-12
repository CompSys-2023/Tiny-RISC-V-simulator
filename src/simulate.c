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

int             registers[32]                        = {0};
DecodeFunction  decode_functions[J_TYPE_OPCODE + 1]  = {0};
ExecuteFunction execute_functions[J_TYPE_OPCODE + 1] = {0};

long int simulate(struct memory* mem, struct assembly* as, int start_addr,
                  FILE* log_file) {
  int      pc                = start_addr;
  bool     running           = true;
  long int instruction_count = 0;

  initializeDecodeFunctions(&decode_functions);
  initializeExecuteFunctions(&decode_functions);

  while (running) {
    uint32_t instruction        = fetch_instruction(mem, pc);
    uint32_t opcode             = instruction & 0x7f;
    void*    decodedInstruction = decode_functions[opcode](instruction);
    execute_instruction(decodedInstruction, opcode, mem, &pc);

    if (log_file != NULL) {
      fprintf(log_file, "PC: %08x, Instruction: %08x\n", pc, instruction);
    }

    pc += 4;
    instruction_count++;
  }
  return instruction_count;
}

// Function to fetch an instruction from memory
uint32_t fetch_instruction(struct memory* mem, int address) { return 0; }

void execute_instruction(void* decoded_instruction, uint32_t opcode,
                         struct memory* mem, int* pc) {
  execute_functions[opcode](decoded_instruction, mem);
}
