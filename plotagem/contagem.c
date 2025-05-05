#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TAMANHO 10000
#define PASSO 1000
#define SORTEIOS 100

// array
int busca_lista(int *lista, int tamanho, int chave, int *comparacoes) {
    for (int i = 0; i < tamanho; i++) {
        (*comparacoes)++;
        if (lista[i] == chave)
            return 1;
    }
    return 0;
}

// arv bin
typedef struct No {
    int valor;
    struct No *esq, *dir;
} No;

No* inserir(No* raiz, int valor) {
    if (raiz == NULL) {
        No* novo = (No*)malloc(sizeof(No));
        novo->valor = valor;
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else
        raiz->dir = inserir(raiz->dir, valor);
    return raiz;
}

int busca_arvore(No* raiz, int chave, int *comparacoes) {
    if (raiz == NULL)
        return 0;
    (*comparacoes)++;
    if (chave == raiz->valor)
        return 1;
    else if (chave < raiz->valor)
        return busca_arvore(raiz->esq, chave, comparacoes);
    else
        return busca_arvore(raiz->dir, chave, comparacoes);
}

void liberar_arvore(No* raiz) {
    if (raiz) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz);
    }
}

// main topada
int main() {
    srand(time(NULL));

    FILE *arquivo = fopen("dados.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo CSV.\n");
        return 1;
    }

    // cabeçalho do arquivo
    fprintf(arquivo, "tamanho,media_comparacoes_lista,media_comparacoes_arvore\n");

    for (int tamanho = PASSO; tamanho <= MAX_TAMANHO; tamanho += PASSO) {
        int *lista = (int*)malloc(tamanho * sizeof(int));
        No *raiz = NULL;

        // aq gera os dados únicos aleatórios
        for (int i = 0; i < tamanho; i++) {
            int valor = rand() % (MAX_TAMANHO * 10);
            lista[i] = valor;
            raiz = inserir(raiz, valor);
        }

        long total_comp_lista = 0, total_comp_arvore = 0;

        for (int i = 0; i < SORTEIOS; i++) {
            int chave = lista[rand() % tamanho];

            int comp_lista = 0;
            busca_lista(lista, tamanho, chave, &comp_lista);
            total_comp_lista += comp_lista;

            int comp_arvore = 0;
            busca_arvore(raiz, chave, &comp_arvore);
            total_comp_arvore += comp_arvore;
        }

        float media_lista = (float)total_comp_lista / SORTEIOS;
        float media_arvore = (float)total_comp_arvore / SORTEIOS;

        fprintf(arquivo, "%d,%.2f,%.2f\n", tamanho, media_lista, media_arvore);

        free(lista);
        liberar_arvore(raiz);
    }

    fclose(arquivo);
    printf("Arquivo 'dados.csv' gerado com sucesso!\n");
    return 0;
}
