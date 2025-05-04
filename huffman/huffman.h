#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * @struct No
 * @brief Estrutura de nó para árvores de Huffman e listas encadeadas.
 *
 * Esta estrutura é utilizada para representar nós em uma árvore de Huffman ,
 * armazenando um caractere , sua frequência e ponteiros para
 * os nós esquerdo, direito e próximo(Para listas encadeadas).
 */
typedef struct No {
    void *caractere;      /**< Ponteiro genérico, que nessa aplicação apontará para um unsigned char */
    int frequencia;       /**< Frequência do caracter */
    struct No *esquerda;  /**< Ponteiro para o nó filho esquerdo */
    struct No *direita;   /**< Ponteiro para o nó filho direito  */
    struct No *proximo;   /**< Ponteiro para o próximo nó (Na parte de lista encadeada) */
} No;

void comprimir(const char *nome_entrada, const char *nome_saida);
void descomprimir(const char *nome_entrada, const char *nome_saida);
void liberar_arvore(No *raiz);

#endif // HUFFMAN_H
