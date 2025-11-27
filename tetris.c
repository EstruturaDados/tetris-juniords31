#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ---------------------------
// Estrutura da peça
// ---------------------------
typedef struct {
    char nome;   // 'I', 'O', 'T', 'L'
    int id;      // identificador único
} Peca;

// ---------------------------
// Variáveis globais
// ---------------------------
int proxID = 0;  // gera IDs únicos

// ---------------------------
// Gera uma peça aleatória
// ---------------------------
Peca gerarPeca() {
    char tipos[4] = { 'I', 'O', 'T', 'L' };
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = proxID++;
    return p;
}

// ---------------------------
// FILA CIRCULAR
// ---------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->qtd++;
    }
}

Peca desenfileirar(Fila *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

// ---------------------------
// PILHA
// ---------------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void empilhar(Pilha *p, Peca x) {
    if (!pilhaCheia(p)) {
        p->itens[++p->topo] = x;
    }
}

Peca desempilhar(Pilha *p) {
    return p->itens[p->topo--];
}

// ---------------------------
// Estado atual
// ---------------------------
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n=== ESTADO ATUAL ===\n");

    printf("Fila de peças:\t");
    int idx = f->inicio;
    for (int i = 0; i < f->qtd; i++) {
        Peca x = f->itens[idx];
        printf("[%c %d] ", x.nome, x.id);
        idx = (idx + 1) % TAM_FILA;
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n======================\n");
}

// ---------------------------
// TROCA DAS 3 PEÇAS
// ---------------------------
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("Erro: não há peças suficientes para a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int posFila = (f->inicio + i) % TAM_FILA;
        Peca aux = f->itens[posFila];
        f->itens[posFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = aux;
    }

    printf("Ação: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// ---------------------------
// PROGRAMA PRINCIPAL
// ---------------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++)
        enfileirar(&fila, gerarPeca());

    int opcao;

    do {
        mostrarEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Reservar peça (fila -> pilha)\n");
        printf("3 - Usar peça reservada (topo da pilha)\n");
        printf("4 - Trocar peça da fila com a da pilha\n");
        printf("5 - Troca múltipla entre fila e pilha (3 peças)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("Peça [%c %d] jogada.\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca());
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca p = desenfileirar(&fila);
                    empilhar(&pilha, p);
                    printf("Peça [%c %d] enviada para a pilha.\n", p.nome, p.id);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("Erro: não é possível reservar.\n");
                }
                break;
            }

            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca p = desempilhar(&pilha);
                    printf("Peça reservada usada: [%c %d]\n", p.nome, p.id);
                } else {
                    printf("Pilha vazia.\n");
                }
                break;
            }

            case 4: {
                if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                    int pos = fila.inicio;
                    Peca aux = fila.itens[pos];
                    fila.itens[pos] = pilha.itens[pilha.topo];
                    pilha.itens[pilha.topo] = aux;
                    printf("Troca simples realizada.\n");
                }
                break;
            }

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}