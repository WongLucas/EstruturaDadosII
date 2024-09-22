#include <stdio.h>
#include <stdlib.h>

typedef struct Funcionario{
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
}Funcionario;

void salvaFuncionario(Funcionario *fun, FILE *out);
Funcionario* leFuncionario(FILE *in);

int main(){
    FILE *fptr = fopen("funcionarios.dat", "wb");

    if(fptr == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    Funcionario lucas = {1,"lucas", "11.11", "12/1/1", 12.1111};
    salvaFuncionario(&lucas, fptr);
    fclose(fptr);

    FILE *fptr1 = fopen("funcionarios.dat", "rb");

    if(fptr1 == NULL){
        printf("Erro ao ler o arquivo\n");
        exit(1);
    }

    Funcionario *aux = leFuncionario(fptr1);

    printf("\tCodigo: %d\n",aux->cod);
    printf("\tNome: %s\n",aux->nome);
    printf("\tCPF: %s\n",aux->cpf);
    printf("\tData de Nascimento: %s\n",aux->data_nascimento);
    printf("\tSalario: %lf\n",aux->salario);
    
    fclose(fptr1);

    return 0;
}

void salvaFuncionario(Funcionario *fun, FILE *out){
    fwrite(&fun->cod, sizeof(int), 1, out);
    fwrite(fun->nome, sizeof(char), sizeof(fun->nome), out);
    fwrite(fun->cpf, sizeof(char), sizeof(fun->cpf), out);
    fwrite(fun->data_nascimento, sizeof(char), sizeof(fun->data_nascimento), out);
    fwrite(&fun->salario, sizeof(double), 1, out);
}

Funcionario* leFuncionario(FILE *in){
    Funcionario *fun = (Funcionario*) malloc(sizeof(Funcionario));

    fread(&fun->cod, sizeof(int), 1, in);
    fread(fun->nome, sizeof(char), sizeof(fun->nome), in);
    fread(fun->cpf, sizeof(char), sizeof(fun->cpf), in);
    fread(fun->data_nascimento, sizeof(char), sizeof(fun->data_nascimento), in);
    fread(&fun->salario, sizeof(double), 1, in);

    return fun;
}