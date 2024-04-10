#include <stdio.h>
#include <stdint.h>

// Função para converter um byte UTF-8 em uma runa de 4 bytes
void utf8_to_rune(uint8_t byte, uint8_t *rune) {
    // Aqui você implementará a lógica para converter um byte UTF-8 em uma runa de 4 bytes
    // Por exemplo, se todos os caracteres em Nust C têm 4 bytes, você pode simplesmente copiar o byte para cada byte da runa
    for (int i = 0; i < 4; i++) {
        rune[i] = byte;
    }
}

void print_binary(uint8_t byte) {
    // Máscara para verificar cada bit no byte
    uint8_t mask = 0x80; // 10000000 em binário

    // Iterar sobre cada bit no byte e imprimir 0 ou 1
    for (int i = 0; i < 8; i++) {
        // Verificar se o bit atual está definido ou não
        if (byte & mask) {
            printf("1");
        } else {
            printf("0");
        }
        // Deslocar a máscara para a direita para verificar o próximo bit
        mask >>= 1;
    }
    printf("\n");
}

int main() {
    // String UTF-8 de exemplo
    char utf8_string[] = "teste em utf-8 olá!";

    printf("Runas de: '%s', tamanho em bytes: %d\n", utf8_string, (int)sizeof(utf8_string));
    for (int i = 0; utf8_string[i] != '\0'; i++) {
        uint8_t byte = utf8_string[i];
        uint8_t rune[4];
        utf8_to_rune(byte, rune);
        printf("Byte UTF-8: %02X, '%c'\n", byte, byte);
        printf("Runa de Nust C: ");
        for (int j = 0; j < 4; j++) {
            printf("%02X ", rune[j]);
        }
        print_binary(byte);
        printf("\n");
    }

    return 0;
}