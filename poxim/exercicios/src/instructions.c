#include "../poxim.h"

static const char *reg_name(uint32_t reg) {
    static const char *names[32] = {
        "zero", "ra", "sp", "gp", "tp",
        "t0", "t1", "t2",
        "s0", "s1",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
        "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
        "t3", "t4", "t5", "t6",
    };
    return names[reg];
}

static void trace_begin(FILE *output, uint32_t pc, const char *mnemonic, const char *operands) {
    fprintf(output, "0x%08x:%-7s%-20s", pc, mnemonic, operands);
}

static void trace_r_op(FILE *output, uint32_t pc, const char *mnemonic, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t a, uint32_t b, uint32_t res, const char *op) {
    char operands[64];
    snprintf(operands, sizeof(operands), "%s,%s,%s", reg_name(rd), reg_name(rs1), reg_name(rs2));
    trace_begin(output, pc, mnemonic, operands);
    fprintf(output, "%s=0x%08x%s0x%08x=0x%08x\n", reg_name(rd), a, op, b, res);
}

static void trace_i_op(FILE *output, uint32_t pc, const char *mnemonic, uint32_t rd, uint32_t rs1, uint32_t imm12, uint32_t a, uint32_t imm_value, uint32_t res, const char *op) {
    char operands[64];
    snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rd), reg_name(rs1), imm12);
    trace_begin(output, pc, mnemonic, operands);
    fprintf(output, "%s=0x%08x%s0x%08x=0x%08x\n", reg_name(rd), a, op, imm_value, res);
}

void execute(CPU *cpu, uint32_t instruction, FILE *output) {
    uint32_t current_pc = cpu->pc;
    cpu->pc = current_pc + 4;

    uint32_t opcode = instruction & 0x7F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x07;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    uint32_t funct7 = (instruction >> 25) & 0x7F;
    int32_t imm = (int32_t)instruction >> 20;
    uint32_t shamt = (instruction >> 20) & 0x1F;
    uint32_t upper = (instruction >> 25) & 0x7F;

    switch (opcode) {
        case 0x03: {
            switch (funct3) {
                case 0x0: {
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    int32_t imm_i = (int32_t)instruction >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_i;

                    if (address < RAM_BASE || address >= RAM_BASE + RAM_SIZE) {
                        fprintf(stderr, "lb fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    int8_t byte = (int8_t)cpu->memory[offset];
                    uint32_t value = (uint32_t)(int32_t)byte;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rd), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "lb", operands);
                    fprintf(output, "%s=mem[0x%08x]=0x%08x\n", reg_name(rd), address, value);
                    set_reg(cpu, rd, value);
                    break;
                }
                case 0x1: {
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    int32_t imm_i = (int32_t)instruction >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_i;

                    if (address < RAM_BASE || address > RAM_BASE + RAM_SIZE - 2) {
                        fprintf(stderr, "lh fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    uint16_t raw = (uint16_t)cpu->memory[offset] | ((uint16_t)cpu->memory[offset + 1] << 8);
                    int16_t half = (int16_t)raw;
                    uint32_t value = (uint32_t)(int32_t)half;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rd), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "lh", operands);
                    fprintf(output, "%s=mem[0x%08x]=0x%08x\n", reg_name(rd), address, value);
                    set_reg(cpu, rd, value);
                    break;
                }
                case 0x2: {
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    int32_t imm_i = (int32_t)instruction >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_i;

                    if (address < RAM_BASE || address > RAM_BASE + RAM_SIZE - 4) {
                        fprintf(stderr, "lw fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    uint32_t value = (uint32_t)cpu->memory[offset] | ((uint32_t)cpu->memory[offset + 1] << 8) | ((uint32_t)cpu->memory[offset + 2] << 16) | ((uint32_t)cpu->memory[offset + 3] << 24);
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rd), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "lw", operands);
                    fprintf(output, "%s=mem[0x%08x]=0x%08x\n", reg_name(rd), address, value);
                    set_reg(cpu, rd, value);
                    break;
                }
                case 0x4: {
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    int32_t imm_i = (int32_t)instruction >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_i;

                    if (address < RAM_BASE || address >= RAM_BASE + RAM_SIZE) {
                        fprintf(stderr, "lbu fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    uint32_t value = cpu->memory[offset];
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rd), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "lbu", operands);
                    fprintf(output, "%s=mem[0x%08x]=0x%08x\n", reg_name(rd), address, value);
                    set_reg(cpu, rd, value);
                    break;
                }
                case 0x5: {
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    int32_t imm_i = (int32_t)instruction >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_i;

                    if (address < RAM_BASE || address > RAM_BASE + RAM_SIZE - 2) {
                        fprintf(stderr, "lhu fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    uint16_t raw = (uint16_t)cpu->memory[offset] | ((uint16_t)cpu->memory[offset + 1] << 8);
                    uint32_t value = raw;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rd), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "lhu", operands);
                    fprintf(output, "%s=mem[0x%08x]=0x%08x\n", reg_name(rd), address, value);
                    set_reg(cpu, rd, value);
                    break;                   
                }
                default:
                    fprintf(stderr, "Erro de implementação (LOAD)\n");
                    return;
            }
            break;
        }
        case 0x33:
            switch (funct3) {
                case 0x0:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t value = a + b;

                        trace_r_op(output, current_pc, "add", rd, rs1, rs2, a, b, value, "+");
                        set_reg(cpu, rd, value);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t res = (uint64_t)a * (uint64_t)b;
                        uint32_t value = (uint32_t)res;

                        trace_r_op(output, current_pc, "mul", rd, rs1, rs2, a, b, value, "*");
                        set_reg(cpu, rd, value);
                    }
                    if (funct7 == 0x20) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t res = a - b;

                        trace_r_op(output, current_pc, "sub", rd, rs1, rs2, a, b, res, "-");
                        set_reg(cpu, rd, res);
                    }                                   
                    break;
                case 0x1:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t shift = cpu->reg[rs2] & 0x1F;
                        uint32_t value = a << shift;

                        char operands[64];
                        snprintf(operands, sizeof(operands), "%s,%s,%s", reg_name(rd), reg_name(rs1), reg_name(rs2));
                        trace_begin(output, current_pc, "sll", operands);
                        fprintf(output, "%s=0x%08x<<%u=0x%08x\n", reg_name(rd), a, shift, value);
                        set_reg(cpu, rd, value);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a_raw = cpu->reg[rs1];
                        uint32_t b_raw = cpu->reg[rs2];
                        uint64_t res = (int64_t)(int32_t)a_raw * (int64_t)(int32_t)b_raw;
                        uint32_t value = (uint32_t)((uint64_t)res >> 32);

                        trace_r_op(output, current_pc, "mulh", rd, rs1, rs2, a_raw, b_raw, value, "*");
                        set_reg(cpu, rd, value);
                    }
                    break;
                case 0x2:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t value = (int32_t)a < (int32_t)b;

                        trace_r_op(output, current_pc, "slt", rd, rs1, rs2, a, b, value, "<");
                        set_reg(cpu, rd, value);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a_raw = cpu->reg[rs1];
                        uint32_t b_raw = cpu->reg[rs2];
                        int64_t res = (int64_t)(int32_t)a_raw * (int64_t)(uint32_t)b_raw;
                        uint32_t value = (uint32_t)((uint64_t)res >> 32);

                        trace_r_op(output, current_pc, "mulhsu", rd, rs1, rs2, a_raw, b_raw, value, "*");
                        set_reg(cpu, rd, value);
                    }
                    break;
                case 0x3:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t value = a < b;

                        trace_r_op(output, current_pc, "sltu", rd, rs1, rs2, a, b, value, "<");
                        set_reg(cpu, rd, value);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint64_t res = (uint64_t)a * (uint64_t)b;
                        uint32_t value = (uint32_t)(res >> 32);

                        trace_r_op(output, current_pc, "mulhu", rd, rs1, rs2, a, b, value, "*");
                        set_reg(cpu, rd, value);
                    }
                    break;
                case 0x4:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t res = a ^ b;

                        trace_r_op(output, current_pc, "xor", rd, rs1, rs2, a, b, res, "^");
                        set_reg(cpu, rd, res);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a_raw = cpu->reg[rs1];
                        uint32_t b_raw = cpu->reg[rs2];
                        int32_t a = (int32_t)a_raw;
                        int32_t b = (int32_t)b_raw;
                        uint32_t value;

                        if (b == 0)
                            value = UINT32_MAX;
                        else if (a == INT32_MIN && b == -1)
                            value = (uint32_t)INT32_MIN;
                        else
                            value = (uint32_t)(a / b);
                            
                        trace_r_op(output, current_pc, "div", rd, rs1, rs2, a_raw, b_raw, value, "/");
                        set_reg(cpu, rd, value);
                    }
                    break;
                case 0x5:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t shift = cpu->reg[rs2] & 0x1F;
                        uint32_t value = a >> shift;

                        char operands[64];
                        snprintf(operands, sizeof(operands), "%s,%s,%s", reg_name(rd), reg_name(rs1), reg_name(rs2));
                        trace_begin(output, current_pc, "srl", operands);
                        fprintf(output, "%s=0x%08x>>%u=0x%08x\n", reg_name(rd), a, shift, value);
                        set_reg(cpu, rd, value);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t value;

                        if (b == 0)
                            value = UINT32_MAX;
                        else
                            value = a / b;
                            
                        trace_r_op(output, current_pc, "divu", rd, rs1, rs2, a, b, value, "/");
                        set_reg(cpu, rd, value);
                    }
                    if (funct7 == 0x20) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t shift = cpu->reg[rs2] & 0x1F;
                        uint32_t value = (uint32_t)((int32_t)a >> shift);

                        char operands[64];
                        snprintf(operands, sizeof(operands), "%s,%s,%s", reg_name(rd), reg_name(rs1), reg_name(rs2));
                        trace_begin(output, current_pc, "sra", operands);
                        fprintf(output, "%s=0x%08x>>%u=0x%08x\n", reg_name(rd), a, shift, value);
                        set_reg(cpu, rd, value);
                    }
                    break;
                case 0x6:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t res = a | b;

                        trace_r_op(output, current_pc, "or", rd, rs1, rs2, a, b, res, "|");
                        set_reg(cpu, rd, res);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a_raw = cpu->reg[rs1];
                        uint32_t b_raw = cpu->reg[rs2];
                        int32_t a = (int32_t)a_raw;
                        int32_t b = (int32_t)b_raw;
                        uint32_t value;

                        if (b == 0)
                            value = a_raw;
                        else if (a == INT32_MIN && b == -1)
                            value = 0;
                        else
                            value = (uint32_t)(a % b);
                            
                        trace_r_op(output, current_pc, "rem", rd, rs1, rs2, a_raw, b_raw, value, "%");
                        set_reg(cpu, rd, value);
                    }
                    break;
                case 0x7:
                    if (funct7 == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];
                        uint32_t res = a & b;

                        trace_r_op(output, current_pc, "and", rd, rs1, rs2, a, b, res, "&");
                        set_reg(cpu, rd, res);
                    }
                    if (funct7 == 0x01) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t b = cpu->reg[rs2];

                        uint32_t value;

                        if (b == 0)
                            value = a;
                        else
                            value = a % b;
                            
                        trace_r_op(output, current_pc, "remu", rd, rs1, rs2, a, b, value, "%");
                        set_reg(cpu, rd, value);
                    }
                    break;
                default:
                    printf("Instrução inválida\n");
                    return ;
                    break;
            }
            break;
        
        case 0x37: {
            uint32_t imm_u = instruction >> 12;
            uint32_t value = imm_u << 12;

            char operands[64];
            snprintf(operands, sizeof(operands), "%s,0x%05x", reg_name(rd), imm_u);
            trace_begin(output, current_pc, "lui", operands);
            fprintf(output, "%s=0x%08x\n", reg_name(rd), value);
            set_reg(cpu, rd, value);
            break;
        }
        
        /////////////////////// instruções imediatas ////////
        case 0x13:
            switch (funct3) {
                case 0x0: {
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    uint32_t old_rs1 = cpu->reg[rs1];
                    uint32_t value = old_rs1 + (uint32_t)imm;
                    char operands[64];
                    snprintf(operands, sizeof(operands),"%s,%s,0x%03x", reg_name(rd), reg_name(rs1), imm12);
                    trace_begin(output, current_pc, "addi", operands);
                    fprintf(output,"%s=0x%08x+0x%08x=0x%08x\n", reg_name(rd), old_rs1, (uint32_t)imm, value);
                    set_reg(cpu, rd, value);
                    break;
                }
                case 0x1:
                    if (upper == 0x00) {
                        uint32_t a = cpu->reg[rs1];
                        uint32_t value = a << shamt;

                        char operands[64];
                        snprintf(operands, sizeof(operands), "%s,%s,%u", reg_name(rd), reg_name(rs1), shamt);
                        trace_begin(output, current_pc, "slli", operands);
                        fprintf(output, "%s=0x%08x<<%u=0x%08x\n", reg_name(rd), a, shamt, value);
                        set_reg(cpu, rd, value);
                    }
                    break;
                case 0x2:
                    printf("slti x%u, x%u, %d\n", rd, rs1, imm);
                    set_reg(cpu, rd, (int32_t)cpu->reg[rs1] < (int32_t)imm); 
                    break;
                case 0x3:
                    printf("sltiu x%u, x%u, %d\n", rd, rs1, imm);
                    set_reg(cpu, rd, (uint32_t)cpu->reg[rs1] < (uint32_t)imm); 
                    break;
                case 0x4:
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    uint32_t a = cpu->reg[rs1];
                    uint32_t value = a ^ (uint32_t)imm;
                    trace_i_op(output, current_pc, "xori", rd, rs1, imm12, a, (uint32_t)imm, value, "^");
                    set_reg(cpu, rd, value);
                    break;
                case 0x5:
                    if (upper == 0x00) {
                        printf("srli x%u, x%u, %u\n", rd, rs1, shamt);
                        set_reg(cpu, rd, cpu->reg[rs1] >> shamt);
                    }
                    if (upper == 0x20) {
                        printf("srai x%u, x%u, %u\n", rd, rs1, shamt);
                        set_reg(cpu, rd, (uint32_t)((int32_t)cpu->reg[rs1] >> shamt));
                    }
                    break;
                case 0x6:
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    uint32_t a = cpu->reg[rs1];
                    uint32_t value = a | (uint32_t)imm;
                    trace_i_op(output, current_pc, "ori", rd, rs1, imm12, a, (uint32_t)imm, value, "|");
                    set_reg(cpu, rd, value);
                    break;
                case 0x7:
                    uint32_t imm12 = (instruction >> 20) & 0xFFF;
                    uint32_t a = cpu->reg[rs1];
                    uint32_t value = a & (uint32_t)imm;
                    trace_i_op(output, current_pc, "andi", rd, rs1, imm12, a, (uint32_t)imm, value, "&");
                    set_reg(cpu, rd, value);
                    break;
                default:
                    return;
                    break;
            }
            break;

        case 0x17: {
            uint32_t imm_u = instruction >> 12;
            uint32_t value = current_pc + (imm_u << 12);
            char operands[64];
            snprintf(operands, sizeof(operands), "%s,0x%05x", reg_name(rd), imm_u);
            trace_begin(output, current_pc, "auipc", operands);
            fprintf(output, "%s=0x%08x+0x%08x=0x%08x\n", reg_name(rd), current_pc, imm_u << 12, value);
            set_reg(cpu, rd, value);
            break;
        }

        case 0x23: {
            switch (funct3) {
                case 0x0: {
                    uint32_t imm12 = ((instruction >> 25) << 5) | ((instruction >> 7) & 0x1F);
                    int32_t imm_s = (int32_t)(imm12 << 20) >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_s;
                    
                    if (address < RAM_BASE || address >= RAM_BASE + RAM_SIZE) {
                        fprintf(stderr, "sb fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    uint32_t value = cpu->reg[rs2];
                    cpu->memory[offset] = value & 0xFF;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rs2), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "sb", operands);
                    fprintf(output, "mem[0x%08x]=0x%02x\n", address, value & 0xFF);
                    break;
                }
                case 0x1: {
                    uint32_t imm12 = ((instruction >> 25) << 5) | ((instruction >> 7) & 0x1F);
                    int32_t imm_s = (int32_t)(imm12 << 20) >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_s;
                    
                    if (address < RAM_BASE || address > RAM_BASE + RAM_SIZE - 2) {
                        fprintf(stderr, "sh fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    uint32_t value = cpu->reg[rs2];
                    cpu->memory[offset] = value & 0xFF;
                    cpu->memory[offset + 1] = (value >> 8) & 0xFF;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rs2), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "sh", operands);
                    fprintf(output, "mem[0x%08x]=0x%04x\n", address, value & 0xFFFF);
                    break;                   
                }
                case 0x2: {
                    uint32_t imm12 = ((instruction >> 25) << 5) | ((instruction >> 7) & 0x1F);
                    int32_t imm_s = (int32_t)(imm12 << 20) >> 20;
                    uint32_t address = cpu->reg[rs1] + (uint32_t)imm_s;
                    uint32_t value = cpu->reg[rs2];

                    if (address < RAM_BASE || address > RAM_BASE + RAM_SIZE - 4) {
                        fprintf(stderr, "SW fora da RAM: 0x%08x\n", address);
                        return;
                    }
                    uint32_t offset = address - RAM_BASE;
                    cpu->memory[offset] = value & 0xFF;
                    cpu->memory[offset + 1] = (value >> 8) & 0xFF;
                    cpu->memory[offset + 2] = (value >> 16) & 0xFF;
                    cpu->memory[offset + 3] = (value >> 24) & 0xFF;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,0x%03x(%s)", reg_name(rs2), imm12, reg_name(rs1));
                    trace_begin(output, current_pc, "sw", operands);
                    fprintf(output, "mem[0x%08x]=0x%08x\n", address, value);
                    break;
                }
                
                default:
                    fprintf(stderr, "Erro de implementação (STORE)\n");
                    return;
            }
            break;
        }
        case 0x63: {
            uint32_t imm_b = (((instruction >> 31) & 0x01) << 11) | (((instruction >> 7) & 0x01) << 10) | (((instruction >> 25) & 0x3F) << 4) | (((instruction >> 8) & 0x0F));
            int32_t signed_imm = (int32_t)(imm_b << 20) >> 20;
            int32_t offset = signed_imm * 2;

            switch (funct3) {
                case 0x0: {
                    uint32_t a = cpu->reg[rs1];
                    uint32_t b = cpu->reg[rs2];
                    int taken = a == b;

                    if (taken)
                        cpu->pc = current_pc + (uint32_t)offset;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rs1), reg_name(rs2), imm_b);
                    trace_begin(output, current_pc, "beq", operands);
                    fprintf(output, "(0x%08x==0x%08x)=%d->pc=0x%08x\n", a, b, taken, cpu->pc);
                    break;
                }
                case 0x1: {
                    uint32_t a = cpu->reg[rs1];
                    uint32_t b = cpu->reg[rs2];
                    int taken = a != b;

                    if (taken)
                        cpu->pc = current_pc + (uint32_t)offset;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rs1), reg_name(rs2), imm_b);
                    trace_begin(output, current_pc, "bne", operands);
                    fprintf(output, "(0x%08x!=0x%08x)=%d->pc=0x%08x\n", a, b, taken, cpu->pc);
                    break;
                }
                case 0x4: {
                    uint32_t a = cpu->reg[rs1];
                    uint32_t b = cpu->reg[rs2];
                    int taken = (int32_t)a < (int32_t)b;

                    if (taken)
                        cpu->pc = current_pc + (uint32_t)offset;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rs1), reg_name(rs2), imm_b);
                    trace_begin(output, current_pc, "blt", operands);
                    fprintf(output, "(0x%08x<0x%08x)=%d->pc=0x%08x\n", a, b, taken, cpu->pc);
                    break;
                }
                case 0x5: {
                    uint32_t a = cpu->reg[rs1];
                    uint32_t b = cpu->reg[rs2];
                    int taken = (int32_t)a >= (int32_t)b;

                    if (taken)
                        cpu->pc = current_pc + (uint32_t)offset;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rs1), reg_name(rs2), imm_b);
                    trace_begin(output, current_pc, "bge", operands);
                    fprintf(output, "(0x%08x>=0x%08x)=%d->pc=0x%08x\n", a, b, taken, cpu->pc);
                    break;

                }
                case 0x6: {
                    uint32_t a = cpu->reg[rs1];
                    uint32_t b = cpu->reg[rs2];
                    int taken = a < b;

                    if (taken)
                        cpu->pc = current_pc + (uint32_t)offset;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rs1), reg_name(rs2), imm_b);
                    trace_begin(output, current_pc, "bltu", operands);
                    fprintf(output, "(0x%08x<0x%08x)=%d->pc=0x%08x\n", a, b, taken, cpu->pc);
                    break;
                }
                case 0x7: {
                    uint32_t a = cpu->reg[rs1];
                    uint32_t b = cpu->reg[rs2];
                    int taken = a >= b;

                    if (taken)
                        cpu->pc = current_pc + (uint32_t)offset;
                    char operands[64];
                    snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rs1), reg_name(rs2), imm_b);
                    trace_begin(output, current_pc, "bgeu", operands);
                    fprintf(output, "(0x%08x>=0x%08x)=%d->pc=0x%08x\n", a, b, taken, cpu->pc);
                    break;

                }
                default:
                    fprintf(stderr, "Erro de implementação (BRANCH)\n");
                    return;
            }
            break;
        }

        case 0x67: {
            uint32_t imm12 = (instruction >> 20) & 0xFFF;
            int32_t imm_i = (int32_t)instruction >> 20;
            uint32_t base = cpu->reg[rs1];
            uint32_t link = current_pc + 4;
            uint32_t target = (base + (uint32_t)imm_i) & ~1u;
            char operands[64];
            snprintf(operands, sizeof(operands), "%s,%s,0x%03x", reg_name(rd), reg_name(rs1), imm12);
            trace_begin(output, current_pc, "jalr", operands);
            fprintf(output, "pc=0x%08x+0x%08x,%s=0x%08x\n", base, (uint32_t)imm_i, reg_name(rd), link);
            set_reg(cpu, rd, link);
            cpu->pc = target;
            break;
        }

        case 0x73: {
            if (instruction == 0x00100073) {
                fprintf(output, "0x%08x:ebreak\n", current_pc);
                cpu->halted = 1;
                break;
            }
            fprintf(stderr, "Erro de implementação (ebreak)\n");
            return;
        }
        
        case 0x6F: {
            uint32_t imm_j = (((instruction >> 31) & 0x01) << 19) | (((instruction >> 12) & 0xFF) << 11) | (((instruction >> 20) & 0x01) << 10) | (((instruction >> 21) & 0x3FF));
            uint32_t offset = (int32_t)imm_j;

            if (imm_j & (1u << 19))
                offset |= (int32_t)0xFFF00000;
            offset <<= 1;
            uint32_t link = current_pc + 4;
            uint32_t target = current_pc + (uint32_t)offset;
            char operands[64];
            snprintf(operands, sizeof(operands), "%s,0x%05x", reg_name(rd), imm_j);
            trace_begin(output, current_pc, "jal", operands);
            fprintf(output, "pc=0x%08x,%s=0x%08x\n", target, reg_name(rd), link);
            set_reg(cpu, rd, link);
            cpu->pc = target;
            break;
        }
        default:
            printf("Instrução não reconhecida");
            break;
    }
}