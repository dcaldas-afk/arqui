.section .text
.globl _start
.option norvc
.option norelax

_start:
    # Base segura para os testes de memória: 0x80007000
    lui s11, 0x80007

    # Inicialização de alguns registradores
    addi ra, zero, 17
    addi sp, zero, 34
    addi gp, zero, 51
    addi tp, zero, 68
    addi t0, zero, 85
    addi t1, zero, 102
    addi t2, zero, 119
    addi s0, zero, 136

    # STORE: 8 combinações de operandos para cada instrução
    sb ra, 0(s11)
    sb sp, 1(s11)
    sb gp, 2(s11)
    sb tp, 3(s11)
    sb t0, 4(s11)
    sb t1, 5(s11)
    sb t2, 6(s11)
    sb s0, 7(s11)
    sh ra, 16(s11)
    sh sp, 18(s11)
    sh gp, 20(s11)
    sh tp, 22(s11)
    sh t0, 24(s11)
    sh t1, 26(s11)
    sh t2, 28(s11)
    sh s0, 30(s11)
    sw ra, 32(s11)
    sw sp, 36(s11)
    sw gp, 40(s11)
    sw tp, 44(s11)
    sw t0, 48(s11)
    sw t1, 52(s11)
    sw t2, 56(s11)
    sw s0, 60(s11)

    # LOAD: 8 combinações de operandos para cada instrução
    lb s1, 0(s11)
    lb a0, 1(s11)
    lb a1, 2(s11)
    lb a2, 3(s11)
    lb a3, 4(s11)
    lb a4, 5(s11)
    lb a5, 6(s11)
    lb a6, 7(s11)
    lh s1, 16(s11)
    lh a0, 18(s11)
    lh a1, 20(s11)
    lh a2, 22(s11)
    lh a3, 24(s11)
    lh a4, 26(s11)
    lh a5, 28(s11)
    lh a6, 30(s11)
    lw s1, 32(s11)
    lw a0, 36(s11)
    lw a1, 40(s11)
    lw a2, 44(s11)
    lw a3, 48(s11)
    lw a4, 52(s11)
    lw a5, 56(s11)
    lw a6, 60(s11)
    lbu s1, 0(s11)
    lbu a0, 1(s11)
    lbu a1, 2(s11)
    lbu a2, 3(s11)
    lbu a3, 4(s11)
    lbu a4, 5(s11)
    lbu a5, 6(s11)
    lbu a6, 7(s11)
    lhu s1, 16(s11)
    lhu a0, 18(s11)
    lhu a1, 20(s11)
    lhu a2, 22(s11)
    lhu a3, 24(s11)
    lhu a4, 26(s11)
    lhu a5, 28(s11)
    lhu a6, 30(s11)

    # Operações R-type e multiplicações
    add zero, ra, sp
    add gp, tp, t0
    add t1, t2, s0
    add s1, a0, a1
    add a2, a3, a4
    add a5, a6, a7
    add s2, s3, s4
    add s5, s6, s7
    sub s8, s9, s10
    sub s11, t3, t4
    sub t5, t6, zero
    sub ra, sp, gp
    sub tp, t0, t1
    sub t2, s0, s1
    sub a0, a1, a2
    sub a3, a4, a5
    sll a6, a7, s2
    sll s3, s4, s5
    sll s6, s7, s8
    sll s9, s10, s11
    sll t3, t4, t5
    sll t6, zero, ra
    sll sp, gp, tp
    sll t0, t1, t2
    slt s0, s1, a0
    slt a1, a2, a3
    slt a4, a5, a6
    slt a7, s2, s3
    slt s4, s5, s6
    slt s7, s8, s9
    slt s10, s11, t3
    slt t4, t5, t6
    sltu zero, ra, sp
    sltu gp, tp, t0
    sltu t1, t2, s0
    sltu s1, a0, a1
    sltu a2, a3, a4
    sltu a5, a6, a7
    sltu s2, s3, s4
    sltu s5, s6, s7
    xor s8, s9, s10
    xor s11, t3, t4
    xor t5, t6, zero
    xor ra, sp, gp
    xor tp, t0, t1
    xor t2, s0, s1
    xor a0, a1, a2
    xor a3, a4, a5
    srl a6, a7, s2
    srl s3, s4, s5
    srl s6, s7, s8
    srl s9, s10, s11
    srl t3, t4, t5
    srl t6, zero, ra
    srl sp, gp, tp
    srl t0, t1, t2
    sra s0, s1, a0
    sra a1, a2, a3
    sra a4, a5, a6
    sra a7, s2, s3
    sra s4, s5, s6
    sra s7, s8, s9
    sra s10, s11, t3
    sra t4, t5, t6
    or zero, ra, sp
    or gp, tp, t0
    or t1, t2, s0
    or s1, a0, a1
    or a2, a3, a4
    or a5, a6, a7
    or s2, s3, s4
    or s5, s6, s7
    and s8, s9, s10
    and s11, t3, t4
    and t5, t6, zero
    and ra, sp, gp
    and tp, t0, t1
    and t2, s0, s1
    and a0, a1, a2
    and a3, a4, a5
    mul a6, a7, s2
    mul s3, s4, s5
    mul s6, s7, s8
    mul s9, s10, s11
    mul t3, t4, t5
    mul t6, zero, ra
    mul sp, gp, tp
    mul t0, t1, t2
    mulh s0, s1, a0
    mulh a1, a2, a3
    mulh a4, a5, a6
    mulh a7, s2, s3
    mulh s4, s5, s6
    mulh s7, s8, s9
    mulh s10, s11, t3
    mulh t4, t5, t6
    mulhsu zero, ra, sp
    mulhsu gp, tp, t0
    mulhsu t1, t2, s0
    mulhsu s1, a0, a1
    mulhsu a2, a3, a4
    mulhsu a5, a6, a7
    mulhsu s2, s3, s4
    mulhsu s5, s6, s7
    mulhu s8, s9, s10
    mulhu s11, t3, t4
    mulhu t5, t6, zero
    mulhu ra, sp, gp
    mulhu tp, t0, t1
    mulhu t2, s0, s1
    mulhu a0, a1, a2
    mulhu a3, a4, a5

    # DIV/DIVU/REM/REMU com divisores não nulos
    addi ra, zero, 100
    addi sp, zero, 1
    div gp, ra, sp
    addi ra, zero, 101
    addi sp, zero, 2
    div tp, ra, sp
    addi ra, zero, 102
    addi sp, zero, 3
    div t0, ra, sp
    addi ra, zero, 103
    addi sp, zero, 4
    div t1, ra, sp
    addi ra, zero, 104
    addi sp, zero, 5
    div t2, ra, sp
    addi ra, zero, 105
    addi sp, zero, 6
    div s0, ra, sp
    addi ra, zero, 106
    addi sp, zero, 7
    div s1, ra, sp
    addi ra, zero, 107
    addi sp, zero, 8
    div a0, ra, sp
    addi ra, zero, 100
    addi sp, zero, 1
    divu a1, ra, sp
    addi ra, zero, 101
    addi sp, zero, 2
    divu a2, ra, sp
    addi ra, zero, 102
    addi sp, zero, 3
    divu a3, ra, sp
    addi ra, zero, 103
    addi sp, zero, 4
    divu a4, ra, sp
    addi ra, zero, 104
    addi sp, zero, 5
    divu a5, ra, sp
    addi ra, zero, 105
    addi sp, zero, 6
    divu a6, ra, sp
    addi ra, zero, 106
    addi sp, zero, 7
    divu a7, ra, sp
    addi ra, zero, 107
    addi sp, zero, 8
    divu s2, ra, sp
    addi ra, zero, 100
    addi sp, zero, 1
    rem s3, ra, sp
    addi ra, zero, 101
    addi sp, zero, 2
    rem s4, ra, sp
    addi ra, zero, 102
    addi sp, zero, 3
    rem s5, ra, sp
    addi ra, zero, 103
    addi sp, zero, 4
    rem s6, ra, sp
    addi ra, zero, 104
    addi sp, zero, 5
    rem s7, ra, sp
    addi ra, zero, 105
    addi sp, zero, 6
    rem s8, ra, sp
    addi ra, zero, 106
    addi sp, zero, 7
    rem s9, ra, sp
    addi ra, zero, 107
    addi sp, zero, 8
    rem s10, ra, sp
    addi ra, zero, 100
    addi sp, zero, 1
    remu s11, ra, sp
    addi ra, zero, 101
    addi sp, zero, 2
    remu t3, ra, sp
    addi ra, zero, 102
    addi sp, zero, 3
    remu t4, ra, sp
    addi ra, zero, 103
    addi sp, zero, 4
    remu t5, ra, sp
    addi ra, zero, 104
    addi sp, zero, 5
    remu gp, ra, sp
    addi ra, zero, 105
    addi sp, zero, 6
    remu tp, ra, sp
    addi ra, zero, 106
    addi sp, zero, 7
    remu t0, ra, sp
    addi ra, zero, 107
    addi sp, zero, 8
    remu t1, ra, sp

    # Instruções imediatas
    addi t0, t1, 3
    addi t2, s0, 6
    addi s1, a0, 9
    addi a1, a2, 12
    addi a3, a4, 15
    addi a5, a6, 18
    addi a7, s2, 21
    addi s3, s4, 24
    slti s5, s6, 3
    slti s7, s8, 6
    slti s9, s10, 9
    slti s11, t3, 12
    slti t4, t5, 15
    slti t6, zero, 18
    slti ra, sp, 21
    slti gp, tp, 24
    sltiu t0, t1, 3
    sltiu t2, s0, 6
    sltiu s1, a0, 9
    sltiu a1, a2, 12
    sltiu a3, a4, 15
    sltiu a5, a6, 18
    sltiu a7, s2, 21
    sltiu s3, s4, 24
    xori s5, s6, 3
    xori s7, s8, 6
    xori s9, s10, 9
    xori s11, t3, 12
    xori t4, t5, 15
    xori t6, zero, 18
    xori ra, sp, 21
    xori gp, tp, 24
    ori t0, t1, 3
    ori t2, s0, 6
    ori s1, a0, 9
    ori a1, a2, 12
    ori a3, a4, 15
    ori a5, a6, 18
    ori a7, s2, 21
    ori s3, s4, 24
    andi s5, s6, 3
    andi s7, s8, 6
    andi s9, s10, 9
    andi s11, t3, 12
    andi t4, t5, 15
    andi t6, zero, 18
    andi ra, sp, 21
    andi gp, tp, 24

    # Shifts imediatos
    slli t2, s0, 1
    slli a1, a2, 2
    slli a5, a6, 3
    slli s3, s4, 4
    slli s7, s8, 5
    slli s11, t3, 6
    slli t6, zero, 7
    slli gp, tp, 8
    srli t2, s0, 1
    srli a1, a2, 2
    srli a5, a6, 3
    srli s3, s4, 4
    srli s7, s8, 5
    srli s11, t3, 6
    srli t6, zero, 7
    srli gp, tp, 8
    srai t2, s0, 1
    srai a1, a2, 2
    srai a5, a6, 3
    srai s3, s4, 4
    srai s7, s8, 5
    srai s11, t3, 6
    srai t6, zero, 7
    srai gp, tp, 8

    # LUI
    lui gp, 0x10000
    lui t1, 0x10001
    lui s1, 0x10002
    lui a2, 0x10003
    lui a5, 0x10004
    lui s2, 0x10005
    lui s5, 0x10006
    lui s8, 0x10007

    # AUIPC
    auipc tp, 0x00010
    auipc t2, 0x00011
    auipc a0, 0x00012
    auipc a3, 0x00013
    auipc a6, 0x00014
    auipc s3, 0x00015
    auipc s6, 0x00016
    auipc s9, 0x00017

    # Bloco extra para elevar a frequência de uso de todos os registradores
    add zero, ra, sp
    add gp, tp, t0
    add t1, t2, s0
    add s1, a0, a1
    add a2, a3, a4
    add a5, a6, a7
    add s2, s3, s4
    add s5, s6, s7
    add s8, s9, s10
    add s11, t3, t4
    add t5, t6, zero
    add ra, sp, gp
    add tp, t0, t1
    add t2, s0, s1
    add a0, a1, a2
    add a3, a4, a5
    add a6, a7, s2
    add s3, s4, s5
    add s6, s7, s8
    add s9, s10, s11
    add t3, t4, t5
    add t6, zero, ra
    add sp, gp, tp
    add t0, t1, t2
    add s0, s1, a0
    add a1, a2, a3
    add a4, a5, a6
    add a7, s2, s3
    add s4, s5, s6
    add s7, s8, s9
    add s10, s11, t3
    add t4, t5, t6
    add zero, ra, sp
    add gp, tp, t0
    add t1, t2, s0
    add s1, a0, a1
    add a2, a3, a4
    add a5, a6, a7
    add s2, s3, s4
    add s5, s6, s7
    add s8, s9, s10
    add s11, t3, t4
    add t5, t6, zero
    add ra, sp, gp
    add tp, t0, t1
    add t2, s0, s1
    add a0, a1, a2
    add a3, a4, a5
    add a6, a7, s2
    add s3, s4, s5
    add s6, s7, s8
    add s9, s10, s11
    add t3, t4, t5
    add t6, zero, ra
    add sp, gp, tp
    add t0, t1, t2
    add s0, s1, a0
    add a1, a2, a3
    add a4, a5, a6
    add a7, s2, s3
    add s4, s5, s6
    add s7, s8, s9
    add s10, s11, t3
    add t4, t5, t6

    # Branches: condições escolhidas para NÃO serem tomadas
    addi ra, zero, 1
    beq zero, ra, .+4
    beq zero, ra, .+8
    beq zero, ra, .+12
    beq zero, ra, .+16
    beq zero, ra, .+20
    beq zero, ra, .+24
    beq zero, ra, .+28
    beq zero, ra, .+32
    bne zero, zero, .+4
    bne zero, zero, .+8
    bne zero, zero, .+12
    bne zero, zero, .+16
    bne zero, zero, .+20
    bne zero, zero, .+24
    bne zero, zero, .+28
    bne zero, zero, .+32
    blt zero, zero, .+4
    blt zero, zero, .+8
    blt zero, zero, .+12
    blt zero, zero, .+16
    blt zero, zero, .+20
    blt zero, zero, .+24
    blt zero, zero, .+28
    blt zero, zero, .+32
    bge zero, ra, .+4
    bge zero, ra, .+8
    bge zero, ra, .+12
    bge zero, ra, .+16
    bge zero, ra, .+20
    bge zero, ra, .+24
    bge zero, ra, .+28
    bge zero, ra, .+32
    bltu zero, zero, .+4
    bltu zero, zero, .+8
    bltu zero, zero, .+12
    bltu zero, zero, .+16
    bltu zero, zero, .+20
    bltu zero, zero, .+24
    bltu zero, zero, .+28
    bltu zero, zero, .+32
    bgeu zero, ra, .+4
    bgeu zero, ra, .+8
    bgeu zero, ra, .+12
    bgeu zero, ra, .+16
    bgeu zero, ra, .+20
    bgeu zero, ra, .+24
    bgeu zero, ra, .+28
    bgeu zero, ra, .+32

    # JAL: salto de +4 cai exatamente na próxima instrução
    jal s1, .+4
    jal a0, .+4
    jal a1, .+4
    jal a2, .+4
    jal a3, .+4
    jal a4, .+4
    jal a5, .+4
    jal a6, .+4

    # JALR: t6 recebe o endereço da próxima instrução após o JALR
    auipc t6, 0
    addi t6, t6, 12
    jalr ra, t6, 0
    auipc t6, 0
    addi t6, t6, 12
    jalr sp, t6, 0
    auipc t6, 0
    addi t6, t6, 12
    jalr gp, t6, 0
    auipc t6, 0
    addi t6, t6, 12
    jalr tp, t6, 0
    auipc t6, 0
    addi t6, t6, 12
    jalr t0, t6, 0
    auipc t6, 0
    addi t6, t6, 12
    jalr t1, t6, 0
    auipc t6, 0
    addi t6, t6, 12
    jalr t2, t6, 0
    auipc t6, 0
    addi t6, t6, 12
    jalr s0, t6, 0

    # Encerramento do simulador
    ebreak
