#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct No {
    void *caractere;
    int frequencia;
    struct No *esquerda;
    struct No *direita;
    struct No *proximo;
} No;

void comprimir(const char *nome_entrada, const char *nome_saida);
void descomprimir(const char *nome_entrada, const char *nome_saida);
void liberar_arvore(No *raiz);

#endif // HUFFMAN_H
