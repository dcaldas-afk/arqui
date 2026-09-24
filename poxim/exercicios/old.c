#include "../poxim.h"

void execute(CPU *cpu, uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x07;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    uint32_t funct7 = (instruction >> 25) & 0x7F;
    int32_t imm = (int32_t)instruction >> 20;
    uint32_t shamt = (instruction >> 20) & 0x1F;
    uint32_t get_31_0 = (instruction >> 31) & 0x1F;
    
    if (opcode == 0x33 && funct3 == 0x7 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] & cpu->reg[rs2];
            printf("AND\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x6 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] | cpu->reg[rs2];
            printf("OR\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x4 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] ^ cpu->reg[rs2];
            printf("XOR\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x7) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] & (uint32_t)imm;
            printf("ANDimm\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x6) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] | (uint32_t)imm;
            printf("ORimm\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x4) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] ^ (uint32_t)imm;
            printf("XORimm\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x1 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] << (cpu->reg[rs2] & 0x1F);
            printf("SLL\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x5 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] >> (cpu->reg[rs2] & 0x1F);
            printf("SRL\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x1 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] << shamt;
            printf("SLLimm\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x5 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = cpu->reg[rs1] >> shamt;
            printf("SRLimm\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x2 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = ((int32_t)cpu->reg[rs1] < (int32_t)cpu->reg[rs2]);
            printf("SLT\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x3 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = (cpu->reg[rs1] < cpu->reg[rs2]);
            printf("SLTU\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x2) {
        if (rd != 0) {
            cpu->reg[rd] = ((int32_t)cpu->reg[rs1] < (int32_t)imm);
            printf("SLTI\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x3) {
        if (rd != 0) {
            cpu->reg[rd] = ((uint32_t)cpu->reg[rs1] < (uint32_t)imm);
            printf("SLTIU\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x0 && funct7 == 0x00) {
        if (rd != 0) {
            cpu->reg[rd] = (cpu->reg[rs1] + cpu->reg[rs2]);
            printf("ADD\n");
        }
    }

    if (opcode == 0x13 && funct3 == 0x0) {
        if (rd != 0) {
           cpu->reg[rd] = (cpu->reg[rs1] + (uint32_t)imm);
           printf("ANDI\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x0 && funct7 == 0x20) {
        if (rd != 0) {
           cpu->reg[rd] = (cpu->reg[rs1] - cpu->reg[rs2]);
           printf("SUB\n");
        }
    }

    if (opcode == 0x33 && funct3 == 0x0 && funct7 == 0x01) {
        if (rd != 0) {
           cpu->reg[rd] = (cpu->reg[rs1] * cpu->reg[rs2]) & get_31_0;
           printf("SUB\n");
        }
    }
}