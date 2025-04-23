#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node {
    unsigned char letra;
    long int freq;
    struct node* next;
    struct node* left;
    struct node* right;
} node;

void insert(unsigned char letra, node** head) {
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
    unsigned char auxl = a->letra;
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
            printf("[%c][%ld]", head->letra, head->freq);
            break;
        }
        printf("[%c][%ld] ->", head->letra, head->freq);
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
    selectionSort(head);
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

void printArvDoc(node* head,FILE* doc) {
    if (!head) {
        return;
    }

    fputc(head->letra,doc);
    printArvDoc(head->left,doc);
    printArvDoc(head->right,doc);
    return;
}

int findPath(node* root, unsigned char letra, char* path, int depth) {
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


int tamanhoArvore(node* arvore) {
    if(arvore == NULL) {
        return 0;
    }
    return 1 + tamanhoArvore(arvore->left) + tamanhoArvore(arvore->right);
}


//NECESSÁRIO ESTUDAR
void printHuffmanCodesToFile(node* head, FILE* entrada, FILE* saida) {
    // 1. Primeiro processamos todo o arquivo para descobrir o lixo
    rewind(entrada);
    int total_bits = 0;
    int ch;
    
    // Calcula o total de bits que seriam gerados
    while ((ch = fgetc(entrada)) != EOF) {
        char path[256];
        if (findPath(head,ch, path, 0)) {
            total_bits += strlen(path);
        }
    }
    
    // Calcula o lixo (bits que faltam para completar o último byte)
    int lixo = (8-(total_bits % 8))%8;
    
    // 2. Escreve o cabeçalho
    // 3 bits de lixo + 13 bits do tamanho da árvore
    int tamanho_arv = tamanhoArvore(head);
    
    // Junta lixo e tamanho em 2 bytes (16 bits)
    unsigned short header = 0;
    header |= (lixo & 0x07) << 13;    // 3 bits de lixo nos 3 mais significativos
    header |= tamanho_arv & 0x1FFF;    // 13 bits do tamanho
    
    // Escreve os 2 bytes do header
    fputc((header >> 8) & 0xFF, saida); // Byte mais significativo
    fputc(header & 0xFF, saida);        // Byte menos significativo
    
    // 3. Escreve a árvore em pré-ordem
    printArvDoc(head,saida);
    
    // 4. Agora escreve os códigos Huffman
    rewind(entrada);
    char buffer = 0;
    int buffer_pos = 0;
    
    while ((ch = fgetc(entrada)) != EOF) {
        char path[256];
        if (!findPath(head, ch, path, 0)) {
            fprintf(stderr, "Caractere não encontrado na árvore: %c\n", ch);
            continue;
        }
        
        for (int j = 0; path[j] != '\0'; j++) {
            if (path[j] == '1') {
                buffer |= (1 << (7 - buffer_pos));
            }
            
            buffer_pos++;
            
            if (buffer_pos == 8) {
                fputc(buffer, saida);
                buffer = 0;
                buffer_pos = 0;
            }
        }
    }
    // Escreve o último byte se houver bits restantes
    if (buffer_pos > 0) {
        fputc(buffer, saida);
    }
}

int is_bit_i_set(unsigned char c, int i)
{
 unsigned char mask = 1 << i;
 return mask & c;
}

//NECESSÁRIO ESTUDAR
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

//NECESSÁRIO ESTUDAR
void readHeader(FILE* file, int* trash, int* treeSize) {
    if (!file || !trash || !treeSize) {
        return;
    }

    // Lê os 2 bytes do header (little-endian)
    unsigned char byte1, byte2;
    if (fread(&byte1, 1, 1, file) != 1 || fread(&byte2, 1, 1, file) != 1) {
        return;
    }

    // Combina os bytes em um valor de 16 bits (considerando a ordem correta)
    unsigned short header = (byte1 << 8) | byte2;  // Big-endian

    // Extrai os 3 bits de lixo (MSB)
    *trash = (header >> 13) & 0x07;  // 0x07 = mascara para 3 bits (00000111)

    // Extrai os 13 bits do tamanho da árvore (LSB)
    *treeSize = header & 0x1FFF;      // 0x1FFF = mascara para 13 bits (0001111111111111)
}

//NECESSÁRIO ESTUDAR
void descompactador(node* arv, FILE* doc, int trash) {
    FILE* output = fopen("C:\\Users\\Joao\\Desktop\\HUFFMAN\\descompactado.txt", "wb");
    if (!output) {
        perror("Erro ao criar arquivo de saída");
        return;
    }

    node* current = arv;
    unsigned char byte;
    int bit_pos = 7; // Começa do bit mais significativo (7)
    long file_size;
    long bytes_read = 0;

    // Obter o tamanho total do arquivo
    fseek(doc, 0, SEEK_END);
    file_size = ftell(doc);
    fseek(doc, 0, SEEK_SET);

    // Pular o cabeçalho (2 bytes) e a árvore (tam bytes)
    fseek(doc, 2, SEEK_SET); // Já lemos o header antes de chamar esta função
    // O restante da árvore já foi lido antes também

    // Posicionar no início dos dados compactados
    fseek(doc, 2 + tamanhoArvore(arv), SEEK_SET);

    while (fread(&byte, 1, 1, doc) == 1) {
        bytes_read++;
        int is_last_byte = (bytes_read == file_size - 2 - tamanhoArvore(arv));

        while (bit_pos >= 0) {
            // Se for o último byte e estivermos nos bits de lixo, pare
            if (is_last_byte && (7 - bit_pos) >= (8 - trash)) {
                break;
            }

            // Pega o bit atual (0 ou 1)
            int bit = (byte >> bit_pos) & 1;
            bit_pos--;

            // Percorre a árvore
            current = (bit == 0) ? current->left : current->right;

            // Se chegou a uma folha
            if (current->left == NULL && current->right == NULL) {
                fputc(current->letra, output);
                current = arv; // Volta para a raiz
            }
        }
        bit_pos = 7; // Reinicia para o próximo byte
    }

    fclose(output);
}


int main() {
    printf("Voce deseja compactar um arquivo ou descompactar um arquivo?\n1 para compactar, 2 para descompactar\n");
    int num;
    scanf("%d",&num);
    if(num==1){
        getchar();
        FILE* arq=fopen("C:\\Users\\Joao\\Desktop\\HUFFMAN\\textograndao.txt","rb");
        if (arq==NULL){
            printf("arquivo bugado\n");
            return 0;
        }
        FILE* arq2=fopen("C:\\Users\\Joao\\Desktop\\HUFFMAN\\Textocompactado.huff","wb");
        node* head = NULL;
        int caractere;
        while((caractere=fgetc(arq))!=EOF){
            insert(caractere, &head);
        }
        huffy(&head);
        printHuffmanCodesToFile(head,arq,arq2);
    }else if(num==2){
        FILE* arq3=fopen("C:\\Users\\Joao\\Desktop\\HUFFMAN\\Textocompactado.huff","rb");
        int lixo,tam=0;
        readHeader(arq3,&lixo,&tam);
        char arvore[tam];
        for (int x=0;x<tam;x++){
            arvore[x]=fgetc(arq3);
        }
        printf("tamanho do lixo:%d",lixo);
        int pos=0;
        node* arvoral=leArvore(arvore,&pos);
        descompactador(arvoral,arq3,lixo);
    }
    return 0;
}