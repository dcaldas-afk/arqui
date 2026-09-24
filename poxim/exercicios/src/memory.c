#include "../poxim.h"

int load_hex(CPU *cpu, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Erro ao tentar abrir o arquivo .hex");
        return -1;
    }
    char token[32];
    uint32_t address = 0;
    int first_address = 1;
    while(fscanf(file, "%31s", token) == 1) {
        if (token[0] == '@') {
            address = (uint32_t)strtoul(token + 1, NULL, 16);
            if(first_address) {
                cpu->pc = address;
                first_address = 0;
            }
            continue;
        }
        uint32_t value = (uint32_t)strtoul(token, NULL, 16);
        if (address < RAM_BASE || address >= RAM_BASE + RAM_SIZE) {
            fprintf(stderr, "Endereço fora da RAM: 0x%08X\n", address);
            fclose(file);
            return -1;
        }
        cpu->memory[address - RAM_BASE] = (uint8_t)value;
        address++;
    }
    fclose(file);
    return 0;
}

uint32_t fetch(CPU *cpu) {
    uint32_t offset = cpu->pc - RAM_BASE;

    return (uint32_t)cpu->memory[offset] | ((uint32_t)cpu->memory[offset + 1] << 8) | ((uint32_t)cpu->memory[offset + 2] << 16) | ((uint32_t)cpu->memory[offset + 3] << 24);
}