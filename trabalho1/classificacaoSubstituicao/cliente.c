#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUANTIDADE_CLIENTES 54

void gerarArquivoDesordenadoClientes() {
    Cliente clientes[QUANTIDADE_CLIENTES] = {
        {29, "Alice Silva", "01/01/1990"},
        {14, "Bruno Souza", "02/02/1985"},
        {76, "Carlos Pereira", "03/03/1978"},
        {75, "Daniela Costa", "04/04/1982"},
        {59, "Eduardo Lima", "05/05/1991"},
        {6, "Fernanda Alves", "06/06/1988"},
        {7, "Gabriel Rocha", "07/07/1983"},
        {74, "Helena Martins", "08/08/1979"},
        {48, "Igor Fernandes", "09/09/1986"},
        {46, "Juliana Ribeiro", "10/10/1984"},
        {10, "Lucas Oliveira", "11/11/1992"},
        {18, "Mariana Santos", "12/12/1987"},
        {56, "Nicolas Mendes", "13/01/1980"},
        {20, "Olivia Araujo", "14/02/1981"},
        {26, "Pedro Carvalho", "15/03/1989"},
        {4, "Quintino Barros", "16/04/1977"},
        {21, "Rafaela Teixeira", "17/05/1983"},
        {65, "Samuel Correia", "18/06/1982"},
        {22, "Tatiana Almeida", "19/07/1985"},
        {49, "Ubirajara Nunes", "20/08/1984"},
        {11, "Valentina Gomes", "21/09/1990"},
        {16, "Wagner Dias", "22/10/1981"},
        {8, "Xavier Pinto", "23/11/1987"},
        {15, "Yasmin Ferreira", "24/12/1986"},
        {5, "Zeca Moraes", "25/01/1979"},
        {19, "Ana Paula", "26/02/1988"},
        {50, "Bruna Figueiredo", "27/03/1985"},
        {55, "Caio Batista", "28/04/1982"},
        {25, "Diana Lopes", "29/05/1983"},
        {66, "Elias Castro", "30/06/1981"},
        {57, "Fabiana Silva", "01/07/1984"},
        {77, "Gustavo Almeida", "02/08/1987"},
        {12, "Heloisa Santos", "03/09/1990"},
        {30, "Isabela Costa", "04/10/1985"},
        {17, "João Pedro", "05/11/1982"},
        {9, "Karina Souza", "06/12/1983"},
        {54, "Leonardo Lima", "07/01/1981"},
        {78, "Marcos Rocha", "08/02/1984"},
        {43, "Natália Ribeiro", "09/03/1987"},
        {38, "Otávio Fernandes", "10/04/1985"},
        {51, "Patrícia Oliveira", "11/05/1982"},
        {32, "Renato Mendes", "12/06/1983"},
        {58, "Sabrina Araujo", "13/07/1981"},
        {13, "Thiago Correia", "14/08/1984"},
        {73, "Ursula Teixeira", "15/09/1987"},
        {79, "Vitor Almeida", "16/10/1985"},
        {27, "Wesley Nunes", "17/11/1982"},
        {1, "Ximena Gomes", "18/12/1983"},
        {3, "Yuri Dias", "19/01/1981"},
        {60, "Zara Pinto", "20/02/1984"},
        {36, "Arthur Ferreira", "21/03/1987"},
        {47, "Beatriz Moraes", "22/04/1985"},
        {0, "César Barros", "23/05/1982"},
        {2, "Débora Batista", "24/06/1983"}
    };

    FILE *out = fopen("arquivoClientesDesordenados.dat", "wb+");

    if(out == NULL){
        printf("Erro ao gerar arquivo de clientes desordenados");
        return;
    }

    fwrite(clientes, sizeof(Cliente), QUANTIDADE_CLIENTES, out);

    fclose(out);
}