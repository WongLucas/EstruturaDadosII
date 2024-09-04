#include <stdio.h>
#include <stdlib.h>

typedef struct No{
    int valor;
    int indice;
    struct No* esq;
    struct No* dir;
} No;

No* criarNo(int valor);
No* criarArvore(int *valores, int N);

void atualiza(No *no);


int main(){
    int a[5] = {4,12,1,32,2};
    No *no = criarArvore(a,5);
    atualiza(no);
    printf("pos(%d) = %d",no->indice, no->valor);
    free(no);

    return 0;
}

No* criarNo(int valor){
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->dir = NULL;
    novoNo->esq = NULL;
    return novoNo;
}


No* criarArvore(int *folhas, int N){
    No **nos = (No**)malloc(sizeof(No*)*N);

    for(int i=0; i<N; i++){
        nos[i] = criarNo(folhas[i]);
        nos[i]->indice = i;
    }

    int count = N;

    while(count > 1){
        int novoCount=0;

        for(int i=0; i<count ; i+=2){
            if(i+1<count){
                No* pai = criarNo(1);
                pai->esq = nos[i];
                pai->dir = nos[i+1];
                nos[novoCount++] = pai;
            }
            else{
                nos[novoCount++] = nos[i];
            }
        }
        count = novoCount;
    }
    No *raiz = nos[0];
    free(nos);
    return raiz;
}

int encontrarVencedor(int valor1, int valor2) {
    return valor1 > valor2 ? valor1 : valor2;
}

void atualiza(No *no){
    if(no == NULL){
        return;
    }
    if((no->esq != NULL) && (no->dir != NULL)){
        atualiza(no->esq);
        atualiza(no->dir);
        if(no->esq->valor < no->dir->valor){
            no->valor = no->esq->valor;
            no->indice = no->esq->indice;
        }else{
            no->valor = no->dir->valor;           
            no->indice = no->dir->indice;
        }
    }
}