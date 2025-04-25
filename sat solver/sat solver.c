#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_VARIAVEIS 100
#define MAX_CLAUSULAS 1000
#define MAX_LITERAIS 100

typedef struct {
    int literais[MAX_LITERAIS];
    int num_literais;
} Clausula;

typedef struct BinaryTree {
    int variavel;
    int atribuicao;
    struct BinaryTree* left;
    struct BinaryTree* right;
} BinaryTree;

Clausula* clausulas[MAX_CLAUSULAS];
int num_variaveis = 0;
int num_clausulas = 0;
int atribuicoes[MAX_VARIAVEIS + 1]; 

BinaryTree* criar_no(int variavel, int atribuicao) {
    BinaryTree* no = (BinaryTree*)malloc(sizeof(BinaryTree));
    no->variavel = variavel;
    no->atribuicao = atribuicao;
    no->left = NULL;
    no->right = NULL;
    return no;
}

void liberar_arvore(BinaryTree* raiz) {
    if (!raiz) return;
    liberar_arvore(raiz->left);
    liberar_arvore(raiz->right);
    free(raiz);
}

bool sat_clausula(Clausula* clausula) {
    bool tem_literal_nao_atribuido = false;
    
    for (int i = 0; i < clausula->num_literais; i++) {
        int literal = clausula->literais[i];
        int variavel = abs(literal);
        
        if (atribuicoes[variavel] == -1) {
            tem_literal_nao_atribuido = true;
            continue;
        }
        
        if ((literal > 0 && atribuicoes[variavel] == 1) || 
            (literal < 0 && atribuicoes[variavel] == 0)) {
            return true;
        }
    }
    
    return tem_literal_nao_atribuido;
}

bool conflito() {
    for (int i = 0; i < num_clausulas; i++) {
        if (!sat_clausula(clausulas[i])) {
            return true;
        }
    }
    return false;
}

bool todas_atribuicoes_feitas() {
    for (int i = 1; i <= num_variaveis; i++) {
        if (atribuicoes[i] == -1) return false;
    }
    return true;
}

bool resolvedor_sat(BinaryTree* raiz) {
    if (conflito()) return false;
    if (todas_atribuicoes_feitas()) return true;

    int variavel = -1;
    for (int i = 1; i <= num_variaveis; i++) {
        if (atribuicoes[i] == -1) {
            variavel = i;
            break;
        }
    }

    if (variavel == -1) return !conflito();

    atribuicoes[variavel] = 1;
    raiz->right = criar_no(variavel, 1);
    if (resolvedor_sat(raiz->right)) return true;

    atribuicoes[variavel] = 0;
    raiz->left = criar_no(variavel, 0);
    if (resolvedor_sat(raiz->left)) return true;

    atribuicoes[variavel] = -1;
    return false;
}

void ler_dimacs(FILE* arquivo) {
    char linha[256];
    int clausulas_lidas = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == 'c') continue;

        if (linha[0] == 'p') {
            sscanf(linha, "p cnf %d %d", &num_variaveis, &num_clausulas);
        } else {
            if (clausulas_lidas >= MAX_CLAUSULAS) {
                exit(1);
            }

            Clausula* clausula = (Clausula*)malloc(sizeof(Clausula));
            clausula->num_literais = 0;

            int literal;
            char* ptr = linha;

            while (sscanf(ptr, "%d", &literal) == 1 && literal != 0) {
                if (clausula->num_literais >= MAX_LITERAIS) {
                    exit(1);
                }

                clausula->literais[clausula->num_literais++] = literal;
                while (*ptr != ' ' && *ptr != '\t' && *ptr != '\0') ptr++;
                while (*ptr == ' ' || *ptr == '\t') ptr++;
            }

            if (clausula->num_literais > 0) {
                clausulas[clausulas_lidas++] = clausula;
            } else {
                free(clausula);
            }
        }
    }

    if (clausulas_lidas != num_clausulas) {
        num_clausulas = clausulas_lidas;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s arquivo.cnf\n", argv[0]);
        return 1;
    }

    for (int i = 0; i <= MAX_VARIAVEIS; i++) {
        atribuicoes[i] = -1;
    }

    FILE* arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    ler_dimacs(arquivo);
    fclose(arquivo);

    BinaryTree* raiz = criar_no(0, -1);
    bool resultado = resolvedor_sat(raiz);

    if (resultado) {
        printf("\nSAT\n");
        for (int i = 1; i <= num_variaveis; i++) {
            printf("x%d = %s\n", i, atribuicoes[i] ? "1" : "0");
        }
    } else {
        printf("\nUNSAT\n");
    }

    liberar_arvore(raiz);

    for (int i = 0; i < num_clausulas; i++) {
        free(clausulas[i]);
    }

    return 0;
}
