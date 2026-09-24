#ifndef POXIM_H
#define POXIM_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define RAM_BASE 0x80000000u
#define RAM_SIZE (32 * 1024)

typedef struct {
    uint32_t reg[32];
    uint32_t pc;
    uint8_t memory[RAM_SIZE];
    uint8_t halted;
} CPU;

static inline void set_reg(CPU *cpu, uint32_t rd, uint32_t value) {
    if (rd != 0)
        cpu->reg[rd] = value;
}

//static void trace_r_op(FILE *output, uint32_t pc, const char *mnemonic, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t a, uint32_t b, uint32_t res, const char *op);

void execute(CPU *cpu, uint32_t instruction, FILE *output);
int load_hex(CPU *cpu, const char *filename);
uint32_t fetch(CPU *cpu);


#endif