#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_ARQ 7

typedef struct Cliente {
    int CodigoCliente;
    char Nome[50];
    char DataNascimento[20];
} Cliente;

typedef struct HeapNode{
    Cliente cliente;
    int indiceArquivo;
} HeapNode;

typedef struct MinHeap{
    HeapNode *nodes;
    int tamanho;
} MinHeap;

void troca(HeapNode *a, HeapNode *b);
void heapify(MinHeap *heap, int i);
HeapNode extrairMinimo(MinHeap *heap);
void inserirHeap(MinHeap *heap, HeapNode node);
void intercalarArquivosOrdenados(FILE *files[], FILE *out);

int main(){
    FILE *files[TAMANHO_ARQ];
    FILE *out;
    out = fopen("saida.dat", "wb");

    MinHeap heap;
    heap.nodes = (HeapNode *)malloc(TAMANHO_ARQ * sizeof(HeapNode));
    heap.tamanho = 0;

    for(int i=0; i<TAMANHO_ARQ; i++){
        char nomeArquivo[20];
        sprintf(nomeArquivo, "%d.dat", i);
        files[i] = fopen(nomeArquivo, "rb");
        Cliente cricri;

        if(fread(&cricri, sizeof(Cliente), 1, files[i]) >= 1){
            HeapNode node = {cricri, i};
            inserirHeap(&heap, node);
        }
    }

    while (heap.tamanho > 0){
        HeapNode minNode = extrairMinimo(&heap);
        fwrite(&minNode.cliente, sizeof(Cliente), 1, out);

        Cliente cliente;
        if (fread(&cliente, sizeof(Cliente), 1, files[minNode.indiceArquivo]) >= 1) {
            HeapNode node = {cliente, minNode.indiceArquivo};
            inserirHeap(&heap, node);
        }
    }

        for (int i = 0; i < TAMANHO_ARQ ; i++) {
        fclose(files[i]);
    }
    fclose(out);
    free(heap.nodes);

    Cliente cli;
    FILE *test = fopen("saida.dat", "rb");
    int i = 0;
    while(fread(&cli, sizeof(Cliente), 1, test) > 0){
        
        printf("(%d) %d \n", ++i, cli.CodigoCliente);
    }


    return 0;
}

void troca(HeapNode *a, HeapNode *b){
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(MinHeap *heap, int i){
    int menor = i;
    int esquerda = 2*i + 1;
    int direita = 2*i + 2;
    
    if(esquerda < heap->tamanho && heap->nodes[esquerda].cliente.CodigoCliente < heap->nodes[menor].cliente.CodigoCliente){
        menor = esquerda;
    }

    if(direita < heap->tamanho && heap->nodes[direita].cliente.CodigoCliente < heap->nodes[menor].cliente.CodigoCliente){
        menor = direita;
    }

    if(menor != i){
        troca(&heap->nodes[i], &heap->nodes[menor]);
        heapify(heap, menor);
    }
}

HeapNode extrairMinimo(MinHeap *heap){
    if(heap->tamanho <= 0){
        printf("Erro ao extrair da Heap");
        exit(EXIT_FAILURE);
    }

    HeapNode raiz = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->tamanho -1];
    heap->tamanho--;
    heapify(heap, 0);

    return raiz;
}

void inserirHeap(MinHeap *heap, HeapNode node){
    if(heap->tamanho == TAMANHO_ARQ){
        printf("Erro ao inserir na HEAP");
        exit(EXIT_FAILURE);
    }

    int i = heap->tamanho;
    heap->tamanho++;
    heap->nodes[i] = node;

    while (i != 0 && heap->nodes[(i - 1) / 2].cliente.CodigoCliente > heap->nodes[i].cliente.CodigoCliente) {
        troca(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}