#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_INICIAL 7
#define FATOR_CARGA 1.0

typedef struct Cliente {
    int codCliente;
    char nome[100];
    struct Cliente *prox;
} Cliente;

typedef struct {
    Cliente **compartimentos;
    int tamanho;
    int numElementos;
} TabelaHash;

unsigned int hash(int codCliente, int tamanho) {
    return codCliente % tamanho;
}

TabelaHash* inicializarTabela(int tamanho) {
    TabelaHash *tabela = (TabelaHash*) malloc(sizeof(TabelaHash));
    tabela->tamanho = tamanho;
    tabela->numElementos = 0;
    tabela->compartimentos = (Cliente**) malloc(tamanho * sizeof(Cliente*));
    for (int i = 0; i < tamanho; i++) {
        tabela->compartimentos[i] = NULL;
    }
    return tabela;
}

void redimensionarTabela(TabelaHash *tabela) {
    int novoTamanho = tabela->tamanho * 2;
    Cliente **novosCompartimentos = (Cliente**) malloc(novoTamanho * sizeof(Cliente*));
    for (int i = 0; i < novoTamanho; i++) {
        novosCompartimentos[i] = NULL;
    }

    for (int i = 0; i < tabela->tamanho; i++) {
        Cliente *atual = tabela->compartimentos[i];
        while (atual != NULL) {
            Cliente *prox = atual->prox;
            unsigned int novoIndice = hash(atual->codCliente, novoTamanho);
            atual->prox = novosCompartimentos[novoIndice];
            novosCompartimentos[novoIndice] = atual;
            atual = prox;
        }
    }

    free(tabela->compartimentos);
    tabela->compartimentos = novosCompartimentos;
    tabela->tamanho = novoTamanho;
}

void inserirCliente(TabelaHash *tabela, int codCliente, const char *nome) {
    if ((float)tabela->numElementos / tabela->tamanho > FATOR_CARGA) {
        redimensionarTabela(tabela);
    }

    unsigned int indice = hash(codCliente, tabela->tamanho);
    Cliente *novoCliente = (Cliente*) malloc(sizeof(Cliente));
    novoCliente->codCliente = codCliente;
    strcpy(novoCliente->nome, nome);
    novoCliente->prox = tabela->compartimentos[indice];
    tabela->compartimentos[indice] = novoCliente;
    tabela->numElementos++;
}

Cliente* buscarCliente(TabelaHash *tabela, int codCliente) {
    unsigned int indice = hash(codCliente, tabela->tamanho);
    Cliente *atual = tabela->compartimentos[indice];
    while (atual != NULL && atual->codCliente != codCliente) {
        atual = atual->prox;
    }
    return atual;
}

void liberarTabela(TabelaHash *tabela) {
    for (int i = 0; i < tabela->tamanho; i++) {
        Cliente *atual = tabela->compartimentos[i];
        while (atual != NULL) {
            Cliente *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(tabela->compartimentos);
    free(tabela);
}

void imprimirTabela(TabelaHash *tabela) {
    for (int i = 0; i < tabela->tamanho; i++) {
        printf("Compartimento %d: ", i);
        Cliente *atual = tabela->compartimentos[i];
        while (atual != NULL) {
            printf("-> [Codigo: %d, Nome: %s] ", atual->codCliente, atual->nome);
            atual = atual->prox;
        }
        printf("-> NULL\n");
    }
}

int main() {
    TabelaHash *tabela = inicializarTabela(TAMANHO_INICIAL);
    int codCliente;
    char nome[100];

    while (1) {
        float fatorCargaAtual = (float)tabela->numElementos / tabela->tamanho;
        printf("Fator de carga atual: %.2f\n", fatorCargaAtual);
        printf("Fator de carga necessário para expansão: %.2f\n", FATOR_CARGA);
        printf("\n");

        printf("Digite o cliente no formato (Codigo Nome) ou 'sair' para encerrar: ");
        scanf("%d %s", &codCliente, nome);

        inserirCliente(tabela, codCliente, nome);
        imprimirTabela(tabela);
        printf("\n");
    }

    liberarTabela(tabela);
    return 0;
}