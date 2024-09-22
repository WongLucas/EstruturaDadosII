#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct Cliente {
    int CodigoCliente;
    char Nome[50];
    char DataNascimento[20];
} Cliente;

void gerarArquivoDesordenadoClientes();

#endif // CLIENTE_H