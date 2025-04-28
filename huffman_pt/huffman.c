#include "huffman.h"
#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   

// Estrutura de nó da árvore de Huffman
typedef struct No {
    unsigned char caractere; // Armazena o caractere (byte)
    int frequencia;          // Frequência de ocorrência do caractere
    struct No *esquerda;      // Filho à esquerda
    struct No *direita;       // Filho à direita
    struct No *proximo;       // Próximo nó na lista (usado para ordenação)
} No;

// Função que cria um novo nó com caractere e frequência
No *criar_no(unsigned char caractere, int frequencia) {
    No *novo_no = (No *)malloc(sizeof(No)); // Aloca memória para o novo nó
    novo_no->caractere = caractere; // Define o caractere do nó
    novo_no->frequencia = frequencia; // Define a frequência do nó
    novo_no->esquerda = NULL; // Inicializa o filho esquerdo como NULL
    novo_no->direita = NULL; // Inicializa o filho direito como NULL
    novo_no->proximo = NULL; // Inicializa o próximo nó como NULL
    return novo_no; // Retorna o novo nó criado
}

// Função para enfileirar um nó na lista de forma ordenada
No *enfileirar(No *lista, No *novo_no) {
    if (!lista || novo_no->frequencia < lista->frequencia) { // Caso a lista esteja vazia ou o novo nó tenha menor frequência que o primeiro
        novo_no->proximo = lista; // O novo nó será a cabeça da lista
        return novo_no;
    }
    No *atual = lista;
    while (atual->proximo && atual->proximo->frequencia <= novo_no->frequencia) { // Percorre a lista até encontrar a posição correta
        atual = atual->proximo;
    }
    novo_no->proximo = atual->proximo; // Insere o novo nó após o nó atual
    atual->proximo = novo_no; // Atualiza o ponteiro do nó atual
    return lista;
}

// Função para construir a árvore de Huffman a partir da tabela de frequências
No *construir_arvore(int tabela_frequencias[256]) {
    No *lista = NULL; // Lista que vai armazenar os nós
    for (int i = 0; i < 256; i++) { // Para cada caractere possível (0-255)
        if (tabela_frequencias[i]) { // Se o caractere tiver frequência maior que zero
            lista = enfileirar(lista, criar_no(i, tabela_frequencias[i])); // Cria um nó e o insere na lista
        }
    }

    while (lista && lista->proximo) { // Enquanto houver mais de um nó na lista
        No *primeiro = lista;
        No *segundo = lista->proximo;
        No *novo_no = criar_no('*', primeiro->frequencia + segundo->frequencia); // Cria um nó interno
        novo_no->esquerda = primeiro; // Define o primeiro nó como filho esquerdo
        novo_no->direita = segundo; // Define o segundo nó como filho direito
        lista = segundo->proximo; // Atualiza a lista para excluir os dois nós que foram combinados
        lista = enfileirar(lista, novo_no); // Insere o novo nó na lista
    }

    return lista; // Retorna o nó final da árvore de Huffman
}

// Função para construir a tabela de códigos binários de Huffman
void construir_tabela(No *raiz, char *caminho, char *tabela[256]) {
    if (!raiz->esquerda && !raiz->direita) { // Caso o nó seja uma folha
        tabela[raiz->caractere] = (char *)malloc(strlen(caminho) + 1); // Aloca espaço para o código binário
        strcpy(tabela[raiz->caractere], caminho); // Copia o código binário para a tabela
        return;
    }
    int tamanho = strlen(caminho); // Calcula o tamanho do código binário até agora
    char caminho_esquerda[tamanho + 2], caminho_direita[tamanho + 2]; // Strings para os códigos das ramificações
    strcpy(caminho_esquerda, caminho); // Copia o código atual para a esquerda
    strcpy(caminho_direita, caminho); // Copia o código atual para a direita
    caminho_esquerda[tamanho] = '0'; // Adiciona o '0' para o filho esquerdo
    caminho_direita[tamanho] = '1'; // Adiciona o '1' para o filho direito
    caminho_esquerda[tamanho + 1] = caminho_direita[tamanho + 1] = '\0'; // Termina as strings

    if (raiz->esquerda)
        construir_tabela(raiz->esquerda, caminho_esquerda, tabela); // Chama recursivamente para o filho esquerdo
    if (raiz->direita)
        construir_tabela(raiz->direita, caminho_direita, tabela); // Chama recursivamente para o filho direito
}

// Função que escreve a árvore de Huffman no arquivo compactado
void escrever_arvore(No *raiz, FILE *saida, int *tamanho) {
    if (!raiz) return; // Se o nó for NULL, retorna
    if (!raiz->esquerda && !raiz->direita) { // Se for uma folha
        if (raiz->caractere == '*' || raiz->caractere == '\\') { // Se o caractere for especial (já escapado)
            fputc('\\', saida); // Escapa o caractere
            (*tamanho)++;
        }
        fputc(raiz->caractere, saida); // Escreve o caractere no arquivo
        (*tamanho)++; // Incrementa o tamanho
    } else {
        fputc('*', saida); // Escreve o marcador de nó interno
        (*tamanho)++;
        escrever_arvore(raiz->esquerda, saida, tamanho); // Escreve a subárvore esquerda
        escrever_arvore(raiz->direita, saida, tamanho); // Escreve a subárvore direita
    }
}

// Função para ler a árvore de Huffman a partir do arquivo
No *ler_arvore(FILE *entrada) {
    int c = fgetc(entrada); // Lê um caractere do arquivo
    if (c == '*') { // Se for um nó interno
        No *no = criar_no('*', 0); // Cria um nó interno
        no->esquerda = ler_arvore(entrada); // Lê a subárvore esquerda
        no->direita = ler_arvore(entrada); // Lê a subárvore direita
        return no;
    } else if (c == '\\') { // Se for um caractere escapado
        int proximo = fgetc(entrada); // Lê o caractere seguinte
        return criar_no(proximo, 0); // Cria um nó com o caractere
    } else {
        return criar_no(c, 0); // Caso contrário, cria um nó com o caractere lido
    }
}

// Função para comprimir um arquivo
void comprimir(const char *nome_entrada, const char *nome_saida) {
    FILE *entrada = fopen(nome_entrada, "rb"); // Abre o arquivo de entrada para leitura binária
    FILE *saida = fopen(nome_saida, "wb"); // Abre o arquivo de saída para escrita binária

    if (!entrada || !saida) { // Se algum dos arquivos não foi aberto corretamente
        printf("Erro ao abrir arquivos.\n"); // Exibe mensagem de erro
        exit(1); // Encerra o programa com erro
    }

    int tabela_frequencias[256] = {0}; // Tabela de frequências de caracteres
    int c;
    while ((c = fgetc(entrada)) != EOF) { // Lê os caracteres do arquivo de entrada até o final
        tabela_frequencias[c]++; // Atualiza a frequência do caractere
    }
    rewind(entrada); // Volta o ponteiro do arquivo de entrada para o início

    No *raiz = construir_arvore(tabela_frequencias); // Constrói a árvore de Huffman a partir da tabela de frequências

    char *tabela[256] = {NULL}; // Tabela para armazenar os códigos de Huffman
    construir_tabela(raiz, "", tabela); // Constrói a tabela de códigos de Huffman

    int tamanho_arvore = 0; // Variável para armazenar o tamanho da árvore
    int tamanho_lixo = 0; // Variável para armazenar o tamanho do lixo (bits não utilizados)

    fseek(saida, 2, SEEK_SET); // Reserva 2 bytes para o cabeçalho

    escrever_arvore(raiz, saida, &tamanho_arvore); // Escreve a árvore de Huffman no arquivo de saída

    unsigned char buffer = 0; // Buffer para armazenar bits durante a compressão
    int bits_preenchidos = 0; // Contador para saber quantos bits foram preenchidos no buffer

    while ((c = fgetc(entrada)) != EOF) { // Lê os caracteres do arquivo de entrada novamente
        char *codigo = tabela[c]; // Obtém o código de Huffman do caractere
        for (int i = 0; codigo[i]; i++) { // Para cada bit do código
            buffer <<= 1; // Desloca o buffer para a esquerda
            if (codigo[i] == '1') // Se o bit for '1', define o bit mais à direita como 1
                buffer |= 1;
            bits_preenchidos++; // Incrementa o contador de bits
            if (bits_preenchidos == 8) { // Se o buffer estiver cheio
                fputc(buffer, saida); // Escreve o byte no arquivo de saída
                bits_preenchidos = 0; // Reseta o contador de bits
                buffer = 0; // Reseta o buffer
            }
        }
    }

    if (bits_preenchidos > 0) { // Se restarem bits no buffer
        buffer <<= (8 - bits_preenchidos); // Desloca os bits restantes para completar o byte
        fputc(buffer, saida); // Escreve o byte final
        tamanho_lixo = 8 - bits_preenchidos; // Atualiza o tamanho do lixo
    }

    rewind(saida); // Volta o ponteiro do arquivo de saída para o início
    unsigned short cabecalho = (tamanho_lixo << 13) | tamanho_arvore; // Cria o cabeçalho com o tamanho do lixo e da árvore
    fputc((cabecalho >> 8) & 0xFF, saida); // Escreve a parte alta do cabeçalho
    fputc(cabecalho & 0xFF, saida); // Escreve a parte baixa do cabeçalho

    fclose(entrada); // Fecha o arquivo de entrada
    fclose(saida); // Fecha o arquivo de saída

    liberar_arvore(raiz); // Libera a memória da árvore de Huffman
    for (int i = 0; i < 256; i++) { // Libera a memória dos códigos de Huffman
        if (tabela[i])
            free(tabela[i]);
    }
}

// Função para descomprimir um arquivo
void descomprimir(const char *nome_entrada, const char *nome_saida) {
    FILE *entrada = fopen(nome_entrada, "rb"); // Abre o arquivo de entrada para leitura binária
    FILE *saida = fopen(nome_saida, "wb"); // Abre o arquivo de saída para escrita binária

    if (!entrada || !saida) { // Se algum dos arquivos não foi aberto corretamente
        printf("Erro ao abrir arquivos.\n"); // Exibe mensagem de erro
        exit(1); // Encerra o programa com erro
    }

    unsigned short cabecalho = 0; // Variável para armazenar o cabeçalho do arquivo
    cabecalho |= fgetc(entrada) << 8; // Lê a parte alta do cabeçalho
    cabecalho |= fgetc(entrada); // Lê a parte baixa do cabeçalho

    int tamanho_lixo = cabecalho >> 13; // Extrai o tamanho do lixo do cabeçalho
    int tamanho_arvore = cabecalho & 0x1FFF; // Extrai o tamanho da árvore do cabeçalho

    No *raiz = ler_arvore(entrada); // Lê a árvore de Huffman a partir do arquivo

    No *atual = raiz; // Aponta para a raiz da árvore
    fseek(entrada, 0, SEEK_END); // Vai até o final do arquivo de entrada
    long tamanho_arquivo = ftell(entrada); // Obtém o tamanho do arquivo
    fseek(entrada, 2 + tamanho_arvore, SEEK_SET); // Volta para a posição após o cabeçalho e a árvore

    long tamanho_conteudo = tamanho_arquivo - (2 + tamanho_arvore); // Calcula o tamanho do conteúdo compactado

    for (long i = 0; i < tamanho_conteudo; i++) { // Para cada byte do conteúdo compactado
        unsigned char byte = fgetc(entrada); // Lê um byte do arquivo
        int bits_para_ler = 8; // Número inicial de bits a ler do byte
        if (i == tamanho_conteudo - 1) // Se for o último byte, ajusta para o tamanho do lixo
            bits_para_ler -= tamanho_lixo;

        for (int j = 7; j >= 8 - bits_para_ler; j--) { // Para cada bit do byte
            int bit = (byte >> j) & 1; // Extrai o bit
            if (bit == 0) // Se for 0, vai para o filho esquerdo
                atual = atual->esquerda;
            else // Se for 1, vai para o filho direito
                atual = atual->direita;

            if (!atual->esquerda && !atual->direita) { // Se chegou a uma folha
                fputc(atual->caractere, saida); // Escreve o caractere na saída
                atual = raiz; // Volta para a raiz
            }
        }
    }

    fclose(entrada); // Fecha o arquivo de entrada
    fclose(saida); // Fecha o arquivo de saída
    liberar_arvore(raiz); // Libera a memória da árvore de Huffman
}

// Função para liberar memória da árvore
void liberar_arvore(No *raiz) {
    if (!raiz) return; // Se o nó for NULL, retorna
    liberar_arvore(raiz->esquerda); // Libera a subárvore esquerda
    liberar_arvore(raiz->direita); // Libera a subárvore direita
    free(raiz); // Libera o nó atual
}


//gcc -Wall -Wextra -O2 -o huffman main.c huffman.c


