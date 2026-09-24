#include "poxim.h"
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

static uint32_t encode_r(
    uint32_t funct7,
    uint32_t rs2,
    uint32_t rs1,
    uint32_t funct3,
    uint32_t rd
) {
    return (funct7 << 25)
         | (rs2 << 20)
         | (rs1 << 15)
         | (funct3 << 12)
         | (rd << 7)
         | 0x33;
}

static uint32_t encode_i(
    int32_t imm,
    uint32_t rs1,
    uint32_t funct3,
    uint32_t rd
) {
    return (((uint32_t)imm & 0xFFF) << 20)
         | (rs1 << 15)
         | (funct3 << 12)
         | (rd << 7)
         | 0x13;
}

static uint32_t encode_shift_i(
    uint32_t upper,
    uint32_t shamt,
    uint32_t rs1,
    uint32_t funct3,
    uint32_t rd
) {
    return (upper << 25)
         | ((shamt & 0x1F) << 20)
         | (rs1 << 15)
         | (funct3 << 12)
         | (rd << 7)
         | 0x13;
}

static void reset_cpu(CPU *cpu)
{
    *cpu = (CPU){0};
}

static void check(
    const char *name,
    uint32_t got,
    uint32_t expected
) {
    if (got == expected) {
        printf("[OK]   %-8s = 0x%08X\n", name, got);
    } else {
        printf("[FAIL] %-8s = 0x%08X | esperado 0x%08X\n",
               name, got, expected);
    }
}

int main(void)
{
    CPU cpu = {0};
    uint32_t inst;

    /* ADD */
    reset_cpu(&cpu);
    cpu.reg[1] = 20;
    cpu.reg[2] = 30;
    inst = encode_r(0x00, 2, 1, 0x0, 3);
    execute(&cpu, inst);
    check("ADD", cpu.reg[3], 50);

    /* ADD com overflow natural de 32 bits */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFFFFFFFF;
    cpu.reg[2] = 1;
    inst = encode_r(0x00, 2, 1, 0x0, 3);
    execute(&cpu, inst);
    check("ADD ovf", cpu.reg[3], 0x00000000);

    /* SUB */
    reset_cpu(&cpu);
    cpu.reg[1] = 5;
    cpu.reg[2] = 10;
    inst = encode_r(0x20, 2, 1, 0x0, 3);
    execute(&cpu, inst);
    check("SUB", cpu.reg[3], 0xFFFFFFFB);

    /* AND */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xF0;
    cpu.reg[2] = 0xCC;
    inst = encode_r(0x00, 2, 1, 0x7, 3);
    execute(&cpu, inst);
    check("AND", cpu.reg[3], 0xC0);

    /* OR */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xF0;
    cpu.reg[2] = 0xCC;
    inst = encode_r(0x00, 2, 1, 0x6, 3);
    execute(&cpu, inst);
    check("OR", cpu.reg[3], 0xFC);

    /* XOR */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xF0;
    cpu.reg[2] = 0xCC;
    inst = encode_r(0x00, 2, 1, 0x4, 3);
    execute(&cpu, inst);
    check("XOR", cpu.reg[3], 0x3C);

    /* SLL */
    reset_cpu(&cpu);
    cpu.reg[1] = 3;
    cpu.reg[2] = 2;
    inst = encode_r(0x00, 2, 1, 0x1, 3);
    execute(&cpu, inst);
    check("SLL", cpu.reg[3], 12);

    /* Teste da máscara & 0x1F */
    reset_cpu(&cpu);
    cpu.reg[1] = 3;
    cpu.reg[2] = 37;      /* 37 & 31 = 5 */
    inst = encode_r(0x00, 2, 1, 0x1, 3);
    execute(&cpu, inst);
    check("SLL mask", cpu.reg[3], 96);

    /* SRL */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFFFFFFF0;
    cpu.reg[2] = 2;
    inst = encode_r(0x00, 2, 1, 0x5, 3);
    execute(&cpu, inst);
    check("SRL", cpu.reg[3], 0x3FFFFFFC);

    /* SRA */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFFFFFFF0; /* -16 */
    cpu.reg[2] = 2;
    inst = encode_r(0x20, 2, 1, 0x5, 3);
    execute(&cpu, inst);
    check("SRA", cpu.reg[3], 0xFFFFFFFC);

    /* SLT */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFFFFFFFF; /* -1 */
    cpu.reg[2] = 1;
    inst = encode_r(0x00, 2, 1, 0x2, 3);
    execute(&cpu, inst);
    check("SLT", cpu.reg[3], 1);

    /* SLTU */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFFFFFFFF;
    cpu.reg[2] = 1;
    inst = encode_r(0x00, 2, 1, 0x3, 3);
    execute(&cpu, inst);
    check("SLTU", cpu.reg[3], 0);

    /* MUL */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFFFFFFFF;
    cpu.reg[2] = 2;
    inst = encode_r(0x01, 2, 1, 0x0, 3);
    execute(&cpu, inst);
    check("MUL", cpu.reg[3], 0xFFFFFFFE);

    /* MULH: -2 * 3 */
    reset_cpu(&cpu);
    cpu.reg[1] = (uint32_t)-2;
    cpu.reg[2] = 3;
    inst = encode_r(0x01, 2, 1, 0x1, 3);
    execute(&cpu, inst);
    check("MULH", cpu.reg[3], 0xFFFFFFFF);

    /* MULHSU: -2 signed * 3 unsigned */
    reset_cpu(&cpu);
    cpu.reg[1] = (uint32_t)-2;
    cpu.reg[2] = 3;
    inst = encode_r(0x01, 2, 1, 0x2, 3);
    execute(&cpu, inst);
    check("MULHSU", cpu.reg[3], 0xFFFFFFFF);

    /* MULHU */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFFFFFFFF;
    cpu.reg[2] = 2;
    inst = encode_r(0x01, 2, 1, 0x3, 3);
    execute(&cpu, inst);
    check("MULHU", cpu.reg[3], 0x00000001);

    /* DIV: -20 / 3 = -6 */
    reset_cpu(&cpu);
    cpu.reg[1] = (uint32_t)-20;
    cpu.reg[2] = 3;
    inst = encode_r(0x01, 2, 1, 0x4, 3);
    execute(&cpu, inst);
    check("DIV", cpu.reg[3], (uint32_t)-6);

    /* DIV por zero */
    reset_cpu(&cpu);
    cpu.reg[1] = 20;
    cpu.reg[2] = 0;
    inst = encode_r(0x01, 2, 1, 0x4, 3);
    execute(&cpu, inst);
    check("DIV /0", cpu.reg[3], UINT32_MAX);

    /* DIV overflow */
    reset_cpu(&cpu);
    cpu.reg[1] = (uint32_t)INT32_MIN;
    cpu.reg[2] = (uint32_t)-1;
    inst = encode_r(0x01, 2, 1, 0x4, 3);
    execute(&cpu, inst);
    check("DIV ovf", cpu.reg[3], 0x80000000);

    /* DIVU */
    reset_cpu(&cpu);
    cpu.reg[1] = 20;
    cpu.reg[2] = 3;
    inst = encode_r(0x01, 2, 1, 0x5, 3);
    execute(&cpu, inst);
    check("DIVU", cpu.reg[3], 6);

    /* REM */
    reset_cpu(&cpu);
    cpu.reg[1] = (uint32_t)-20;
    cpu.reg[2] = 3;
    inst = encode_r(0x01, 2, 1, 0x6, 3);
    execute(&cpu, inst);
    check("REM", cpu.reg[3], (uint32_t)-2);

    /* REM overflow */
    reset_cpu(&cpu);
    cpu.reg[1] = (uint32_t)INT32_MIN;
    cpu.reg[2] = (uint32_t)-1;
    inst = encode_r(0x01, 2, 1, 0x6, 3);
    execute(&cpu, inst);
    check("REM ovf", cpu.reg[3], 0);

    /* REMU */
    reset_cpu(&cpu);
    cpu.reg[1] = 20;
    cpu.reg[2] = 3;
    inst = encode_r(0x01, 2, 1, 0x7, 3);
    execute(&cpu, inst);
    check("REMU", cpu.reg[3], 2);

    /* ADDI */
    reset_cpu(&cpu);
    cpu.reg[1] = 20;
    inst = encode_i(-5, 1, 0x0, 3);
    execute(&cpu, inst);
    check("ADDI", cpu.reg[3], 15);

    /* SLTI */
    reset_cpu(&cpu);
    cpu.reg[1] = (uint32_t)-5;
    inst = encode_i(2, 1, 0x2, 3);
    execute(&cpu, inst);
    check("SLTI", cpu.reg[3], 1);

    /* SLTIU: 1 < 0xFFFFFFFF */
    reset_cpu(&cpu);
    cpu.reg[1] = 1;
    inst = encode_i(-1, 1, 0x3, 3);
    execute(&cpu, inst);
    check("SLTIU", cpu.reg[3], 1);

    /* XORI */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xF0;
    inst = encode_i(0x0F, 1, 0x4, 3);
    execute(&cpu, inst);
    check("XORI", cpu.reg[3], 0xFF);

    /* ORI */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xF0;
    inst = encode_i(0x0F, 1, 0x6, 3);
    execute(&cpu, inst);
    check("ORI", cpu.reg[3], 0xFF);

    /* ANDI */
    reset_cpu(&cpu);
    cpu.reg[1] = 0xFF;
    inst = encode_i(0x0F, 1, 0x7, 3);
    execute(&cpu, inst);
    check("ANDI", cpu.reg[3], 0x0F);

    /* SLLI */
    reset_cpu(&cpu);
    cpu.reg[1] = 1;
    inst = encode_shift_i(0x00, 4, 1, 0x1, 3);
    execute(&cpu, inst);
    check("SLLI", cpu.reg[3], 16);

    /* SRLI */
    reset_cpu(&cpu);
    cpu.reg[1] = 0x80000000;
    inst = encode_shift_i(0x00, 4, 1, 0x5, 3);
    execute(&cpu, inst);
    check("SRLI", cpu.reg[3], 0x08000000);

    /* SRAI */
    reset_cpu(&cpu);
    cpu.reg[1] = 0x80000000;
    inst = encode_shift_i(0x20, 4, 1, 0x5, 3);
    execute(&cpu, inst);
    check("SRAI", cpu.reg[3], 0xF8000000);

    /* x0 nunca pode ser alterado */
    reset_cpu(&cpu);
    cpu.reg[1] = 10;
    cpu.reg[2] = 20;
    inst = encode_r(0x00, 2, 1, 0x0, 0);
    execute(&cpu, inst);
    check("x0", cpu.reg[0], 0);

    return 0;
}