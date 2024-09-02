#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 6

void writeSequence(FILE *out);
void readSequence(FILE *in);

int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

int particoes=0;

void makeOrderedArchive(int *array, int n);

int main(){
    FILE *originalSequence;
    originalSequence = fopen("sequence.dat", "w+b");

    if(originalSequence == NULL){
        printf("erro ao entrar acessar o arquivo;");
        exit(1);
    }

    writeSequence(originalSequence); //Escrevendo um arquivo com a sequencia desordenada
    rewind(originalSequence);

    readSequence(originalSequence);

    fclose(originalSequence);

    FILE *teste = fopen("nomeArquivo(1).dat", "rb");
    if(teste == NULL){
        printf("erro ao entrar acessar o teste;");
        exit(1);
    }

    int num[MEM_SIZE];
    fread(num, sizeof(int), MEM_SIZE, teste);
    for(int i=0 ; i<MEM_SIZE; i++){
        printf("%d ", num[i]);
    }

    fclose(teste);
    return 0;
}

void writeSequence(FILE *out){

    printf("\tEscrevendo a sequencia desordenada...\n\n");

    int sequencia[53] = {
        29,14,76,75,59,6,7,74,48,46,10,18,
        56,20,26,4,21,65,22,49,11,16,8,15,
        5,19,50,55,25,66,57,77,12,30,17,9,
        54,78,43,38,51,32,58,13,73,79,27,1,
        3,60,36,47,31    
        };

    for(int i=0; i<53; i++){
        fwrite(&sequencia[i], sizeof(int), 1, out);
    }
}

void readSequence(FILE *in){
    printf("\tLendo a sequencia desordenada...\n\n");
    int num[MEM_SIZE];
    double aux;
    while((aux = fread(num, sizeof(int), MEM_SIZE, in))>0){
        printf("\tDados lidos do arquivo(%1.f): ", aux);
        for(int i=0 ; i<aux; i++){
            printf("%d ", num[i]);
        }
        printf("\n\tSequencia ordenada: ");
        qsort(num, (int)aux, sizeof(int), compare);
        for(int i=0 ; i<aux; i++){
            printf("%d ", num[i]);
        }
        printf("\n\n");
        makeOrderedArchive(num, (int)aux);
        particoes++;
    }
}

void makeOrderedArchive(int *array, int n){
    char nomeArquivo[50];
    sprintf(nomeArquivo, "nomeArquivo(%d).dat", particoes);

    FILE *out = fopen(nomeArquivo, "w+b");

    if(out == NULL){
        printf("Erro ao gerar particao.");
        exit(1);
    }

    fwrite(array, sizeof(int), n, out);

    fclose(out);
}