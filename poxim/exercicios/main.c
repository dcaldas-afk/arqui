#include "poxim.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Uso: %s arquivo.hex arquivo.out\n", argv[0]);
        return 1;
    }

    CPU cpu = {0};

    if (load_hex(&cpu, argv[1]) != 0)
        return 1;

    FILE *output = fopen(argv[2], "w");

    if (output == NULL) {
        perror("Erro ao criar arquivo de saida");
        return 1;
    }

    while (!cpu.halted) {
        uint32_t instruction = fetch(&cpu);
        execute(&cpu, instruction, output);
    }

    fclose(output);

    return 0;
}