#include "huffman.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <c|d> <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }
    if (argv[1][0] == 'c')
        comprimir(argv[2], argv[3]);
    else if (argv[1][0] == 'd')
        descomprimir(argv[2], argv[3]);
    else
        printf("Opção inválida!\n");
    return 0;
}
