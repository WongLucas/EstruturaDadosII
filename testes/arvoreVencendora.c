#include <stdio.h>
#include <stdlib.h>

#define MAX_ARQUIVOS 100
#define HIGH_VALUE 10000000

typedef struct{
    int valor;
    int indice;
} HeapNode;

void trocar(HeapNode *a, HeapNode *b);
void construirHeap(HeapNode *heap, int N);
void ajustarHeap(HeapNode *heap, int N, int i);
void removerMin(HeapNode *heap, int *N);
void intercalaArquivos(FILE **arquivos, int num_arquivos, FILE *saida);

int main(){
    int numArquivos = 9;
    FILE* arquivos[MAX_ARQUIVOS];

    arquivos[0] = fopen("nomeArquivo(0).dat", "rb");
    arquivos[1] = fopen("nomeArquivo(1).dat", "rb");
    arquivos[2] = fopen("nomeArquivo(2).dat", "rb");
    arquivos[3] = fopen("nomeArquivo(3).dat", "rb");
    arquivos[4] = fopen("nomeArquivo(4).dat", "rb");
    arquivos[5] = fopen("nomeArquivo(5).dat", "rb");
    arquivos[6] = fopen("nomeArquivo(6).dat", "rb");
    arquivos[7] = fopen("nomeArquivo(7).dat", "rb");
    arquivos[8] = fopen("nomeArquivo(8).dat", "rb");

    FILE* saida = fopen("tudoJuntinho.dat", "wb");

    intercalaArquivos(arquivos, numArquivos, saida);

    for(int i=0; i<numArquivos; i++){
        fclose(arquivos[i]);
    }

    fclose(saida);
    
    FILE *teste = fopen("tudoJuntinho.dat", "rb");
    
    int i;
    while(fread(&i, sizeof(int), 1, teste)){
        printf("%d ",i);
    }

    fclose(teste);

    return 0;
}

void trocar(HeapNode *a, HeapNode *b){
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void construirHeap(HeapNode *heap, int N){
    for(int i=(N/2)-1; i>=0; i--){
        ajustarHeap(heap,N,i);
    }
}

void ajustarHeap(HeapNode *heap, int N, int i){
    int menor = i;
    int esquerda = i*2 +1;
    int direita = i*2 +2;

    if((esquerda<N) && (heap[esquerda].valor < heap[menor].valor)){
        menor = esquerda;
    }
    if((direita<N) && (heap[direita].valor < heap[menor].valor)){
        menor = direita;
    }
    if(menor != i){
        trocar(&heap[i], &heap[menor]);
        ajustarHeap(heap, N, menor);
    }
}

void removerMin(HeapNode *heap, int *N){
    if (*N <= 0){
        return;
    }
    heap[0] = heap[(*N) - 1];
    (*N)--;
    ajustarHeap(heap, *N, 0);
}

void intercalaArquivos(FILE **arquivos, int num_arquivos, FILE *saida){
    int N = num_arquivos;
    HeapNode heap[MAX_ARQUIVOS];
    int valores_atuais[MAX_ARQUIVOS];

    for(int i=0; i<N; i++){
        if(fread(&valores_atuais[i], sizeof(int), 1, arquivos[i])){
            heap[i].valor = valores_atuais[i];
            heap[i].indice = i;
        }
        else{
            heap[i].valor = HIGH_VALUE;
        }
    }

    construirHeap(heap, N);

    while(N>0){
        HeapNode vencedor = heap[0];
        
        if (vencedor.valor != HIGH_VALUE) {
            fwrite(&vencedor.valor, sizeof(int), 1, saida);
        }

        int indiceVencedor = vencedor.indice;
        if(fread(&valores_atuais[indiceVencedor], sizeof(int), 1, arquivos[indiceVencedor]) != 0){
            heap[0].valor = valores_atuais[indiceVencedor];
        }else{
            heap[0].valor = HIGH_VALUE;
        }

        ajustarHeap(heap, N, 0);

        if(heap[0].valor == HIGH_VALUE){
            removerMin(heap, &N);
        }
    }
}