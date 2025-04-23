#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node {
    char letra;
    int freq;
    struct node* next;
    struct node* left;
    struct node* right;
} node;

void insert(char letra, node** head) {
    node* current = *head;
    while (current != NULL) {
        if (current->letra == letra) {
            current->freq++;
            return;
        }
        current = current->next;
    }
    node* new_node = malloc(sizeof(node));
    new_node->letra = letra;
    new_node->freq = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->next = *head;
    *head = new_node;
    return;
}

void swap(node* a, node* b) {
    char auxl = a->letra;
    int auxf = a->freq;
    a->letra = b->letra;
    a->freq = b->freq;
    b->letra = auxl;
    b->freq = auxf;

    node* temp_left = a->left;
    node* temp_right = a->right;
    a->left = b->left;
    a->right = b->right;
    b->left = temp_left;
    b->right = temp_right;
}

void selectionSort(node** head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }
    node* current = *head;
    while (current != NULL) {
        node* minNode = current;
        node* temp = current->next;
        while (temp != NULL) {
            if (temp->freq < minNode->freq) {
                minNode = temp;
            }
            temp = temp->next;
        }
        swap(current, minNode);
        current = current->next;
    }
}

void printLista(node* head) {
    while (head != NULL) {
        if (head->next == NULL) {
            printf("[%c][%d]", head->letra, head->freq);
            break;
        }
        printf("[%c][%d] ->", head->letra, head->freq);
        head = head->next;
    }
    printf("\n");
    return;
}

void remover(node** head, node* no) {
    if (*head == no) {
        *head = (*head)->next;
    }

    node* current = *head;
    node* prox = current->next;

    while (prox != NULL && prox != no) {
        current = prox;
        prox = prox->next;
    }
    if (prox == no) {
        current->next = prox->next;
    }
    return;
}

void huffy(node** head) {
    if ((*head)->next == NULL) {
        return;
    }
    selectionSort(head);
    node* current = *head;
    node* prox = current->next;
    node* new_node = malloc(sizeof(node));
    new_node->letra = '*';
    new_node->freq = current->freq + prox->freq;
    new_node->left = current;
    new_node->right = prox;
    *head = prox->next;
    new_node->next = *head;
    *head = new_node;
    huffy(head);
}

void printArv(node* head) {
    if (!head) {
        return;
    }
    printf("%c", head->letra);
    printArv(head->left);
    printArv(head->right);
    return;
}

int findPath(node* root, char letra, char* path, int depth) {
    if (root == NULL) {
        return 0;
    }
    if (root->letra == letra && root->left == NULL && root->right == NULL) {
        path[depth] = '\0';
        return 1;
    }
    path[depth] = '0';
    if (findPath(root->left, letra, path, depth + 1)) {
        return 1;
    }
    path[depth] = '1';
    if (findPath(root->right, letra, path, depth + 1)) {
        return 1;
    }
    return 0;
}

char* decimalParaBinario(int decimal) {
    // Caso especial para 0
    if (decimal == 0) {
        char* binario = (char*)malloc(2 * sizeof(char));
        binario[0] = '0';
        binario[1] = '\0';
        return binario;
    }
    
    // Calcula o número de bits necessários
    int temp = decimal;
    int numBits = 0;
    while (temp > 0) {
        temp /= 2;
        numBits++;
    }
    
    // Aloca memória para a string binária (+1 para o caractere nulo)
    char* binario = (char*)malloc((numBits + 1) * sizeof(char));
    binario[numBits] = '\0'; // Terminador nulo
    
    // Preenche a string com os bits
    for (int i = numBits - 1; i >= 0; i--) {
        binario[i] = (decimal % 2) + '0'; // Converte resto para caractere '0' ou '1'
        decimal /= 2;
    }
    
    return binario;
}

char* decimalParaBinarioFormatado(int decimal) {
    // Obtém a representação binária original
    char* binario = decimalParaBinario(decimal);
    int tamanho = strlen(binario);
    
    // Aloca espaço para 13 dígitos + espaço + terminador nulo (15 no total)
    char* binarioFormatado = (char*)malloc(15 * sizeof(char));
    binarioFormatado[14] = '\0';
    
    // Preenche com zeros à esquerda primeiro
    int zeros = 13 - tamanho;
    for (int i = 0; i < zeros; i++) {
        binarioFormatado[i] = '0';
    }
    
    // Copia o binário original após os zeros
    strcpy(binarioFormatado + zeros, binario);
    
    // Libera a memória do binário original
    free(binario);
    
    // Agora vamos inserir o espaço após o 5º bit
    // Movemos os caracteres 5..12 para 6..13
    for (int i = 13; i > 5; i--) {
        binarioFormatado[i] = binarioFormatado[i-1];
    }
    
    // Insere o espaço na posição 5
    binarioFormatado[5] = ' ';
    
    return binarioFormatado;
}

int tamanhoArvore(node* arvore) {
    if(arvore == NULL) {
        return 0;
    }
    return 1 + tamanhoArvore(arvore->left) + tamanhoArvore(arvore->right);
}



int printHuffmanCodes(node* root, char* frase) {
    char buffer[9] = {0};
    int buffer_pos = 0;

    for (size_t i = 0; i < strlen(frase); i++) {
        char path[256];
        findPath(root, frase[i], path, 0);
        

        for (int j = 0; path[j] != '\0'; j++) {
            buffer[buffer_pos++] = path[j];
            if (buffer_pos == 8) {
                buffer[8] = '\0';
                printf("%s ", buffer);
                buffer_pos = 0;
            }
        }
    }
    
    // Imprime os bits restantes se houver
    if (buffer_pos > 0) {
        buffer[buffer_pos] = '\0';
        printf("%s", buffer);
    }
    return 8-buffer_pos;
}


node* leArvore(const char* str, int* pos) {
    if (str[*pos] == '\0') {
        return NULL;
    }

    // Cria novo nó
    node* novo = malloc(sizeof(node));
    novo->letra = str[*pos];
    novo->freq = 0;
    novo->next = NULL;
    
    (*pos)++;  // Avança para o próximo caractere

    if (novo->letra == '*') {
        // Nó interno - tem filhos
        novo->left = leArvore(str, pos);
        novo->right = leArvore(str, pos);
    } else {
        // Folha - não tem filhos
        novo->left = NULL;
        novo->right = NULL;
    }

    return novo;
}

void descompactador(node* arv, const char* texto, int* pos) {
    node* atual = arv;
    
    while (texto[*pos] != '\0') {
        // Pula espaços
        if (texto[*pos] == ' ') {
            (*pos)++;
            continue;
        }
        
        // Navega na árvore
        if (texto[*pos] == '0') {
            atual = atual->left;
        } else if (texto[*pos] == '1') {
            atual = atual->right;
        }
        
        (*pos)++;
        
        // Se chegou numa folha, imprime e reinicia
        if (atual->left == NULL && atual->right == NULL) {
            printf("%c", atual->letra);
            atual = arv;  // Volta para a raiz
        }
    }
}


int main() {
    printf("Voce deseja compactar uma string ou descompactar uma frase binaria?\n1 para compactar, 2 para descompactar\n");
    int num;
    scanf("%d",&num);
    if(num==1){
        getchar();
        char frase[10000];
        printf("Digite a frase a ser compactada:\n");
        node* head = NULL;
        scanf("%s", frase);
        for (size_t i = 0; i < strlen(frase); i++) {
            insert(frase[i], &head);
        }
        printLista(head);
        huffy(&head);
        printf("\n");
        int lixo=printHuffmanCodes(head, frase);
        printf("\n");
        char* lixo2=decimalParaBinario(lixo);
        int i=tamanhoArvore(head);
        char* tam=decimalParaBinarioFormatado(i);
        printf("%s %s ",lixo2,tam);
        printArv(head);
        printf("\n");
    }else if(num==2){
        printf("Digite a árvore:\n");
        char frase2[10000];
        scanf("%s",frase2);
        int pos;
        pos=0;
        node* arv2=leArvore(frase2,&pos);
        //printArv(arv2);
        getchar();
        int pos2=0;
        char frase3[10000];
        printf("Digite a frase binaria\n");
        fgets(frase3,sizeof(frase3),stdin);
        descompactador(arv2,frase3,&pos2);

    }

    
    return 0;
}