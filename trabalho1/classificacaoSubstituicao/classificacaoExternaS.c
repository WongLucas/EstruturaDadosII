#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define TAMANHO_MEMORIA 6

typedef struct {
    Cliente *Cliente;
    bool Congelado;
} Memoria;

Memoria** inicializaMemoria(FILE *in);
int menorPosicaoValorDescongelado(Memoria **M);
int quantidadeMemoriaCongeladas(Memoria **M);
void descongelaMemoria(Memoria **M);
int classificacaoExternaPorSubstituicao(FILE *in);
void casoEspecial(Memoria **M, int particao);
FILE* criaArquivoParticao(int particao);

int main() {
    gerarArquivoDesordenadoClientes();
    
    FILE *arqDes = fopen("arquivoClientesDesordenados.dat", "rb"); // Abrindo arquivo desordenado
    if (arqDes == NULL) {
        printf("Não foi possível abrir o arquivo para leitura!\n");
        exit(EXIT_FAILURE);
    }

    int particoes = classificacaoExternaPorSubstituicao(arqDes);
    //printf("\n\n%d", particoes);

    FILE *teste;
    Cliente Aux;
    for(int i=0; i<=particoes; i++){
        char nomeArquivo[20];
        sprintf(nomeArquivo, "%d.dat", i);
        teste = fopen(nomeArquivo, "rb");
        printf("\n\tParticao (%d)\n", i);
        while (fread(&Aux, sizeof(Cliente), 1, teste) > 0)
        {
            printf("%d\t%s\t%s\t\n", Aux.CodigoCliente, Aux.Nome, Aux.DataNascimento);
        }
        fclose(teste);
    }

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

void casoEspecial(Memoria **M, int particao){
Memoria **CopiaM = (Memoria**)malloc(sizeof(Memoria*) * TAMANHO_MEMORIA);
    for (int i = 0; i < TAMANHO_MEMORIA; i++) {
        CopiaM[i] = (Memoria*)malloc(sizeof(Memoria));
        // Copiar os valores de M[i] para CopiaM[i]
        CopiaM[i]->Cliente = (Cliente*)malloc(sizeof(Cliente));
        CopiaM[i]->Cliente->CodigoCliente = M[i]->Cliente->CodigoCliente;
        strcpy(CopiaM[i]->Cliente->Nome, M[i]->Cliente->Nome);
        strcpy(CopiaM[i]->Cliente->DataNascimento, M[i]->Cliente->DataNascimento);
        CopiaM[i]->Congelado = !M[i]->Congelado;
    }

    int posicao;
    FILE *arqParticao = criaArquivoParticao(++particao);

    printf("(Particao especial: ");
    while (quantidadeMemoriaCongeladas(CopiaM) != TAMANHO_MEMORIA)
    {
        posicao=menorPosicaoValorDescongelado(CopiaM);
        CopiaM[posicao]->Congelado = true;
        fwrite(CopiaM[posicao]->Cliente, sizeof(Cliente), 1, arqParticao);
        printf("%d\t", CopiaM[posicao]->Cliente->CodigoCliente);
    }
    printf(")");    

    for (int i = 0; i < TAMANHO_MEMORIA; i++) {
        free(CopiaM[i]->Cliente);
        free(CopiaM[i]);
    }
    free(CopiaM);
    fclose(arqParticao);
}

FILE* criaArquivoParticao(int particao){
    char nomeArquivo[20];
    sprintf(nomeArquivo, "%d.dat", particao);
    
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao criar o arquivo %s\n", nomeArquivo);
        exit(1);
    }
    
    return arquivo;
}


int classificacaoExternaPorSubstituicao(FILE *in){
    Memoria **M = inicializaMemoria(in);

    int particao=0;
    int posicao;
    bool arquivoAcabou=false;
    Cliente novoCliente;
    FILE *arqParticao = criaArquivoParticao(particao);

    int especial=0;

    while(!arquivoAcabou){
        posicao=menorPosicaoValorDescongelado(M);

        if(posicao == -1){
            printf("Nova particao\n");
            particao++;
            fclose(arqParticao);
            arqParticao = criaArquivoParticao(particao);
            descongelaMemoria(M);
            posicao=menorPosicaoValorDescongelado(M);
        }

        fwrite(M[posicao]->Cliente, sizeof(Cliente), 1, arqParticao);
        printf("%d\t", M[posicao]->Cliente->CodigoCliente);

        if((fread(&novoCliente, sizeof(Cliente), 1, in)) > 0){
            if((novoCliente.CodigoCliente) < (M[posicao]->Cliente->CodigoCliente)){
                M[posicao]->Congelado = true;
            }
            M[posicao]->Cliente->CodigoCliente = novoCliente.CodigoCliente;
            strcpy(M[posicao]->Cliente->Nome, novoCliente.Nome);
            strcpy(M[posicao]->Cliente->DataNascimento, novoCliente.DataNascimento);
        }else{
            if(quantidadeMemoriaCongeladas(M) != 0){
                especial++;
                casoEspecial(M, particao);
            }
            M[posicao]->Congelado = true;
            arquivoAcabou = true;
        }
    }

    while (quantidadeMemoriaCongeladas(M) != TAMANHO_MEMORIA)
    {
        posicao=menorPosicaoValorDescongelado(M);
        M[posicao]->Congelado = true;
        fwrite(M[posicao]->Cliente, sizeof(Cliente), 1, arqParticao);
        printf("%d\t", M[posicao]->Cliente->CodigoCliente);
    }
    fclose(arqParticao);

    return particao + especial;
}