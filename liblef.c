#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"
#include "eventos.h"
#include <string.h>

/* 
 * Cria uma LEF vazia 
 */
lef_t * cria_lef (){
    lef_t *lef = malloc(sizeof(lef_t));
    if (lef == NULL){
        return NULL;
    }

    lef->Primeiro = NULL;
    return lef;
}

/* 
 * Destroi a LEF, sempre retorna NULL. 
 * Para evento eh necessario executar a funcao destroidados para liberar os dados 
 * que representam o evento.
 */ 
lef_t * destroi_lef (lef_t *l){

    /* Destroi eventos */
    nodo_lef_t *aux = l->Primeiro, *remove;

    while (aux != NULL){

        remove = aux;
        aux = aux->prox;

        /* free evento->dados */
        void *ptr = remove->evento->dados;
        remove->evento->destroidados(ptr);

        /* Free evento_t*/
        free(remove->evento);
        /* Free nodo_lef_t*/
        free(remove);
    }
    
    /* Destroi lef */
    free(l);
    return NULL;
}

/* 
 * Adiciona um evento na primeira posicao da LEF. Uma nova copia
 * da estrutura evento sera feita. 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */ 
int adiciona_inicio_lef (lef_t *l, evento_t *evento){
    
    nodo_lef_t *nodo = malloc(sizeof(nodo_lef_t));
    if (nodo == NULL)
        return 0;

    evento_t *e = malloc(sizeof(evento_t));
    if (e == NULL){
        return 0;
    }
    memcpy(e, evento, sizeof(evento_t));

    e->dados = malloc(evento->tamanho);
    if (e->dados == NULL){
        return 0;
    }
    memcpy(e->dados, evento->dados, evento->tamanho);


    nodo->evento = e;
    

    nodo->prox = l->Primeiro;
    l->Primeiro = nodo;
    return 1;
}

/* 
 * Adiciona um evento de acordo com o valor evento->tempo na LEF. 
 * Uma nova copia da estrutura evento sera feita 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef  (lef_t *l, evento_t *evento){

    nodo_lef_t *nodo = malloc(sizeof(nodo_lef_t)); 
    if (nodo == NULL){
        return 0;
    }

    evento_t *e = malloc(sizeof(evento_t));
    if (e == NULL){
        return 0;
    }
    memcpy(e, evento, sizeof(evento_t));

    e->dados = malloc(evento->tamanho);
    if (e->dados == NULL){
        return 0;
    }
    memcpy(e->dados, evento->dados, evento->tamanho);

    nodo->evento = e;
    
    /* Vazio */
    if (l->Primeiro == NULL){
        nodo->prox  = NULL;
        l->Primeiro = nodo;
        return 1;
    }

    /* Primeiro elemento maior que o atual */
    if (evento->tempo < l->Primeiro->evento->tempo){
        nodo->prox   = l->Primeiro;
        l->Primeiro  = nodo;
        return 1;
    }

    /* Encontra o elemento anterio a posição que o evento deve ser inserido*/
    nodo_lef_t *aux = l->Primeiro;
    while (aux->prox != NULL && aux->prox->evento->tempo < evento->tempo){
        aux = aux->prox;
    }
    
    nodo->prox = aux->prox;
    aux->prox = nodo;

    return 1;
}

/* 
 * Retorna o primeiro evento da LEF. A responsabilidade por desalocar
 * a memoria associado eh de quem chama essa funcao 
 */
evento_t * obtem_primeiro_lef (lef_t *l){
    if (l->Primeiro == NULL){
        return NULL;
    }
    
    nodo_lef_t *aux = l->Primeiro;
    evento_t *e = aux->evento;
    l->Primeiro = l->Primeiro->prox;
    free(aux);
    return e;
}
