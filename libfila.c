#include "libfila.h"
#include <stdio.h>
#include <stdlib.h>

fila_t * cria_fila(){

fila_t *fila;
fila = malloc(sizeof(fila_t));
if (fila == NULL){
    return NULL;
}

fila->ini = NULL;
fila->fim = NULL;
fila->tamanho = 0;

return fila;
}

/*
 * Remove todos os elementos da fila, libera espaco e devolve NULL.
 * Não apaga os ponteiros de prox e prev apenas a chave
 */
fila_t * destroi_fila (fila_t *f){
    if (!fila_vazia(f)){

        nodo_f_t *atual = f->fim;
        while (atual->prev != NULL){
            
            /* Apagando chave*/
            atual->chave = 0;
        
            atual = atual->prev;
            free(atual->prox);
        }

        f->tamanho = 0;
        atual->chave = 0;
        free(atual);

    }
    
    free(f);

    return NULL;
}

/*
 * Retorna 1 se a fila esta vazia e 0 caso contrario.
 */
int fila_vazia (fila_t *f){
    return f->ini == NULL;
}

/*
 * Retorna o tamanho da fila, isto eh, o numero de elementos presentes nela.
 */
int tamanho_fila (fila_t *f){
    return f->tamanho;
}
/*
 * Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int insere_fila (fila_t *f, int elemento){
    
    nodo_f_t *nodo;
    nodo = malloc(sizeof(nodo_f_t));
    if (nodo==NULL){
        return 0;
    }

    nodo->chave = elemento;
    nodo->prox = NULL;

    if (fila_vazia(f)){
        nodo->prev = NULL;
        f->ini = nodo;
    }
    else{
        nodo->prev = f->fim;
        f->fim->prox = nodo;
    }
    f->fim = nodo;
    f->tamanho++;

    return 1;    
}

/*
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int retira_fila (fila_t *f, int *elemento){


    if (fila_vazia(f)){
        return 0;
    }

    *elemento = f->ini->chave;

    if (tamanho_fila(f) == 1){
        free(f->ini);
        f->ini = NULL;
        f->fim = NULL;
        f->tamanho--;
        return 1;
    }
    
    f->ini = f->ini->prox;
    free(f->ini->prev);
    f->ini->prev = NULL;
    f->tamanho--;
    return 1;
}

