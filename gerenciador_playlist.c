#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Musica {
    char titulo[100];
    char artista[100];
    struct Musica *prox;
} Musica;

typedef struct Playlist {
    Musica *inicio;
} Playlist;

Playlist* criarPlaylist() {
    Playlist *p = (Playlist*)malloc(sizeof(Playlist));
    p->inicio = NULL;
    return p;
}

Musica* novaMusica(const char *titulo, const char *artista) {
    Musica *m = (Musica*)malloc(sizeof(Musica));
    strcpy(m->titulo, titulo);
    strcpy(m->artista, artista);
    m->prox = NULL;
    return m;
}

void inserirMusica(Playlist *p, const char *titulo, const char *artista) {
    Musica *m = novaMusica(titulo, artista);
    if (p->inicio == NULL) {
        p->inicio = m;
        return;
    }
    Musica *atual = p->inicio;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = m;
}

void inserirApos(Playlist *p, const char *tituloAlvo, const char *titulo, const char *artista) {
    Musica *atual = p->inicio;
    while (atual != NULL) {
        if (strcmp(atual->titulo, tituloAlvo) == 0) {
            Musica *nova = novaMusica(titulo, artista);
            nova->prox = atual->prox;
            atual->prox = nova;
            return;
        }
        atual = atual->prox;
    }
    printf("Música '%s' não encontrada.\n", tituloAlvo);
}

void removerMusica(Playlist *p, const char *titulo) {
    Musica *atual = p->inicio, *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            if (anterior == NULL) {
                p->inicio = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            printf("\nMusica '%s' removida da playlist.\n", titulo);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    printf("Musica '%s' não encontrada.\n", titulo);
}

void moverMusica(Playlist *p, const char *titulo, int novaPos) {
    if (novaPos < 1) {
        printf("Posição inválida.\n");
        return;
    }

    Musica *atual = p->inicio, *anterior = NULL, *alvo = NULL, *alvoAnt = NULL;
    int pos = 1;

    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            alvo = atual;
            alvoAnt = anterior;
            break;
        }
        anterior = atual;
        atual = atual->prox;
        pos++;
    }

    if (!alvo) {
        printf("Musica '%s' não encontrada.\n", titulo);
        return;
    }

    if (alvoAnt == NULL) {
        p->inicio = alvo->prox;
    } else {
        alvoAnt->prox = alvo->prox;
    }

    if (novaPos == 1) {
        alvo->prox = p->inicio;
        p->inicio = alvo;
    } else {
        Musica *temp = p->inicio;
        int i = 1;
        while (temp != NULL && i < novaPos - 1) {
            temp = temp->prox;
            i++;
        }
        if (temp == NULL) {
            printf("Posição maior que o tamanho da lista. Inserindo no final.\n");
            inserirMusica(p, alvo->titulo, alvo->artista);
            free(alvo);
            return;
        }
        alvo->prox = temp->prox;
        temp->prox = alvo;
    }
}

void exibirPlaylist(Playlist *p) {
    Musica *atual = p->inicio;
    int pos = 1;
    printf("\nPlaylist atual:\n");
    while (atual != NULL) {
        printf("%d. %s - %s\n", pos++, atual->titulo, atual->artista);
        atual = atual->prox;
    }
    if (pos == 1) printf("(vazia)\n");
}

void liberarPlaylist(Playlist *p) {
    Musica *atual = p->inicio;
    while (atual != NULL) {
        Musica *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(p);
}

int main() {
    Playlist *playlist = criarPlaylist();

    inserirMusica(playlist, "Bohemian Rhapsody", "Queen");
    inserirMusica(playlist, "Imagine", "John Lennon");
    inserirMusica(playlist, "Billie Jean", "Michael Jackson");
    inserirMusica(playlist, "This Is Gospel", "Panic At The Disco");

    exibirPlaylist(playlist);

    inserirApos(playlist, "Bohemian Rhapsody", "Taj Mahal", "Jorge Ben Jor");
    removerMusica(playlist, "Imagine");
    moverMusica(playlist, "Billie Jean", 1);  

    exibirPlaylist(playlist);

    liberarPlaylist(playlist);
    return 0;
}
