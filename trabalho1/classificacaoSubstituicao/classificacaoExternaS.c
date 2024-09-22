#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define TAMANHO_MEMORIA 6

typedef struct {
    Cliente *Cliente;
    bool Congelado;
} Memoria;

Memoria** inicializaMemoria(FILE *in);
int menorPosicaoValorDescongelado(Memoria **M);
int quantidadeMemoriaCongeladas(Memoria **M);
void descongelaMemoria(Memoria **M);
void classificacaoExternaPorSubstituicao(FILE *in);

int main() {
    gerarArquivoDesordenadoClientes();
    
    FILE *arqDes = fopen("arquivoClientesDesordenados.dat", "rb"); // Abrindo arquivo desordenado
    if (arqDes == NULL) {
        printf("Não foi possível abrir o arquivo para leitura!\n");
        exit(EXIT_FAILURE);
    }

    classificacaoExternaPorSubstituicao(arqDes);

    fclose(arqDes);

    return 0;
}

Memoria** inicializaMemoria(FILE *in) {
    Memoria **M = (Memoria**)malloc(sizeof(Memoria*) * TAMANHO_MEMORIA);
    if(M == NULL){
        printf("Erro ao malocar M");
        exit(EXIT_FAILURE);
    }
 
    for (int i = 0; i < TAMANHO_MEMORIA; i++) {
        M[i] = (Memoria*)malloc(sizeof(Memoria));
        M[i]->Cliente = (Cliente*)malloc(sizeof(Cliente));

        if((M[i]==NULL) || (M[i]->Cliente==NULL)){
        printf("Erro ao malocar M[i] || M[i]->Cliente");
        exit(EXIT_FAILURE);
        }

        fread(M[i]->Cliente, sizeof(Cliente), 1, in);
        M[i]->Congelado = false;
    }
    return M;
}

void descongelaMemoria(Memoria **M){
    for(int i=0; i<TAMANHO_MEMORIA; i++){
        M[i]->Congelado = false;
    }
}

int menorPosicaoValorDescongelado(Memoria **M){
    int menor=INT_MAX;
    int posicao = -1;

    for(int i=0; i<TAMANHO_MEMORIA; i++){
        if(((M[i]->Cliente->CodigoCliente) < menor) && !(M[i]->Congelado)){
            menor = M[i]->Cliente->CodigoCliente; // Atualiza o menor valor
            posicao = i;
        }
    }
    return posicao;
}

int quantidadeMemoriaCongeladas(Memoria **M){
    int count=0;
    for(int i=0; i<TAMANHO_MEMORIA; i++){
        if(M[i]->Congelado == true){
            count++;
        }
    }
    return count;
}


void classificacaoExternaPorSubstituicao(FILE *in){
    Memoria **M = inicializaMemoria(in);

    int particao=0;
    int posicao;
    bool arquivoAcabou=false;
    Cliente novoCliente;

    while(!arquivoAcabou){
        posicao=menorPosicaoValorDescongelado(M);

        if(posicao == -1){
            printf("Nova particao\n");
            descongelaMemoria(M);
            posicao=menorPosicaoValorDescongelado(M);
        }

        printf("%d\t", M[posicao]->Cliente->CodigoCliente);

        if((fread(&novoCliente, sizeof(Cliente), 1, in)) > 0){
            if(novoCliente.CodigoCliente < M[posicao]->Cliente->CodigoCliente){
                M[posicao]->Congelado = true;
            }
            M[posicao]->Cliente->CodigoCliente = novoCliente.CodigoCliente;
        }else{
            M[posicao]->Congelado = true;
            arquivoAcabou = true;
        }
    }
    while (quantidadeMemoriaCongeladas(M) != TAMANHO_MEMORIA)
    {
        posicao=menorPosicaoValorDescongelado(M);
        M[posicao]->Congelado = true;
        printf("%d\t", M[posicao]->Cliente->CodigoCliente);
    }
    

}