#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MEM_SIZE 6

typedef struct MemoriaAgua{
    int valor;
    bool congelado;
}MemoriaAgua;

void gerarArquivoDesordenado(FILE *out);
void lerArquivo(FILE *in);//função para ajudar a debugar
void descongelarMemoria(MemoriaAgua **M);
MemoriaAgua** inicializarMemoria(int *a);
int menorValorNaoCongelado(MemoriaAgua** M);
void selecaoComSubstituicao(FILE *in);

int main(){
    FILE *fptrSequencia = fopen("sequencia.dat", "w+b");
    
    if (fptrSequencia == NULL){
        printf("Erro ao gerar arquivo de sequencia");
        exit(1);
    }

    gerarArquivoDesordenado(fptrSequencia);
    rewind(fptrSequencia);
    //lerArquivo(fptrSequencia);

    selecaoComSubstituicao(fptrSequencia);

    fclose(fptrSequencia);

    return 0;
}

void gerarArquivoDesordenado(FILE *out){
    int sequencia[54] = {
        29,14,76,75,59,6,7,74,48,46,10,18,
        56,20,26,4,21,65,22,49,11,16,8,15,
        5,19,50,55,25,66,57,77,12,30,17,9,
        54,78,43,38,51,32,58,13,73,79,27,1,
        3,60,36,47,31,80
        };
    for(int i=0; i<54; i++){
        fwrite(&sequencia[i], sizeof(int), 1, out);
    }
}

void lerArquivo(FILE *in){
    int aux;
    while(fread(&aux, sizeof(int), 1, in)){
        printf("%d ", aux);
    }
}

MemoriaAgua** inicializarMemoria(int *a){
    MemoriaAgua **M = (MemoriaAgua**)malloc(sizeof(MemoriaAgua)*MEM_SIZE);
    for (int i=0; i<MEM_SIZE; i++){
        M[i] = (MemoriaAgua*)malloc(sizeof(MemoriaAgua));
        M[i]->valor = a[i];
        M[i]->congelado = false;        
    }
    return M;
}

void descongelarMemoria(MemoriaAgua **M){
    for(int i=0; i<MEM_SIZE; i++){
        M[i]->congelado = false;
    }
}

void selecaoComSubstituicao(FILE *in){
    MemoriaAgua **M;
    int aux[MEM_SIZE];
    int particao = 0;

    fread(aux, sizeof(int), MEM_SIZE, in);
    M = inicializarMemoria(aux);

    int pos;
    int novoValor;
    bool acabouArquivo = false;

    while(1){
        if(fread(&novoValor, sizeof(int), 1, in) <= 0){
            pos = menorValorNaoCongelado(M);
            break;
        }
        pos = menorValorNaoCongelado(M);
        if(pos == -1){
            printf("\nNova Particao\n");
            descongelarMemoria(M);
            continue;
        }else{
            printf("%d ", M[pos]->valor);
            if(novoValor < M[pos]->valor){
                M[pos]->valor = novoValor;
                M[pos]->congelado = true;
            }else{
                M[pos]->valor = novoValor;
            }
        }
    }

    while((pos = menorValorNaoCongelado(M)) != -1){
        printf("%d ", M[pos]->valor);
        M[pos]->congelado = true;
    }
}

int menorValorNaoCongelado(MemoriaAgua** M){
    int posicao = -1;
    int menor = 100000;
    for(int i=0; i<MEM_SIZE; i++){
        if(!(M[i]->congelado) && (M[i]->valor < menor)){
            menor = M[i]->valor;
            posicao = i;
        }
    }
    return posicao;
}
