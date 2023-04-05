#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "liblef.h"
#include "eventos.h"
#include "entidades.h"
#include "libconjunto.h"

/*********************** Valores da simulação ************************/
#define N_TAM_MUNDO 20000
#define N_PESSOAS 100
#define N_LOCAIS 8
#define N_RUMORES 30
#define FIM_DO_MUNDO  100
/*********************************************************************/

/*********************** Flags ************************/
#define FIM -1
#define CONTINUA 1
#define ERRO 0
/******************************************************/

int main(){
    
    /* Usar um rand aleatorio*/
    /*
    time_t t;
    srand((unsigned) time(&t));
    */
    
    /*Rand Fixo*/
    srand(0);

    /* Criando mundo */
    mundo_t *m = cria_mundo(N_PESSOAS,N_LOCAIS, N_RUMORES, N_TAM_MUNDO); 
    if (m == NULL){
        printf("Erro: A criação de mundo falhou\n");
        return 1;
    }

    /* Criando lista de eventos */
    lef_t *lef = cria_lef(); 
    if (lef == NULL){
        destroi_mundo(m);
        printf("Erro: A criação de lef falhou\n");
        return 1;
    }

    /* Adiciona Fim do mundo na lista */
    evento_t *fim = cria_fim(FIM_DO_MUNDO);
    if (fim == NULL){
        destroi_mundo(m);
        destroi_lef(lef);
        printf("Erro: A criação de lef falhou\n");
        return 1;
    }
    adiciona_inicio_lef(lef,fim);
    destroi_fim(fim);

    /* Inicializa os primeiros eventos do mundo */
    if(!inicializa_eventos_mundo(m, lef)){
        printf("Erro: A inicialização de eventos falhou\n");
        destroi_mundo(m);
        destroi_lef(lef);
        return 1;
    }
    
    /* Resolve eventos da lista de evento ate FIM */
    evento_t *atual = obtem_primeiro_lef(lef);
    while (atual != NULL){

        /* Muda o tempo do mundo pro momento do evento */
        if(!atualiza_tempo(m, atual->tempo))
            return 1;
        
        /* Resolve o evento*/
        switch (trata_evento(atual, m, lef)){
        case FIM:
            atual = NULL;
            break;
        case  CONTINUA:
            atual = obtem_primeiro_lef(lef);
            break; 
        
        case ERRO:
            printf("Um erro ocorreu durante o tratamento");
            return 1;
        }
    }

    return 0;
}
