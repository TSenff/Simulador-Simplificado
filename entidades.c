#include "entidades.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/********************Pessoa********************/

pessoa_t* cria_pessoa(int id, conjunto_t *rumores_mundo){
    
    pessoa_t *p = malloc(sizeof(pessoa_t));
    if (p == NULL){
        return NULL;
    }
    p->id      = id;
    p->extro   = rand() % 101;
    p->pac     = rand() % 101;
    p->idade   = (rand() % 83) + 18;
    p->rumores = cria_subconjunto(rumores_mundo, (rand() % 5)+1);

    return p;
}

pessoa_t* destroi_pessoa(pessoa_t *p){
    /* Apagando dados */
    p->extro = 0;
    p->pac   = 0;
    p->idade = 0;

    /* Liberando Memoria */
    destroi_conjunto(p->rumores);
    free(p);
    return NULL;
}

/********************Local********************/

local_t* cria_local(int id, int tam_mundo){
    
    local_t *l = malloc(sizeof(local_t));
    if (l == NULL){
        return NULL;
    }

    l->max     = (rand() % 26) + 5;
    l->pessoas = malloc(sizeof(int)*l->max);
    if (l->pessoas == NULL){
        free(l);
        return NULL;
    }
    
    l->fila    = cria_fila();
    if (l->fila == NULL){
        free(l->pessoas);
        free(l);
        return NULL;
    }
    
    l->pos    = malloc(sizeof(cord_t));
    if (l->pos == NULL){
        free(l->pessoas);
        destroi_fila(l->fila);
        free(l);
        return NULL;
    }

    l->id        = id;
    l->n_pessoas = 0;

    /* O mundo vai da cordenada 0 - (tam_mundo-1) */
    l->pos->x     = rand() % tam_mundo;
    l->pos->y     = rand() % tam_mundo;

    return l;
}

local_t* destroi_local(local_t *l){
    
    /* Apagando dados */
    l->id      = 0;
    l->max     = 0;
    l->pos->x   = 0;
    l->pos->y   = 0;

    /* Libera */
    free(l->pessoas);
    free(l->pos);
    destroi_fila(l->fila);
    
    free(l);

    return NULL;
}

int distancia(local_t *l1, local_t *l2){
    return sqrt(pow((l1->pos->x - l2->pos->x),2) + pow((l1->pos->y - l2->pos->y),2));
} 

int local_cheio(local_t *l){
    return l->max <= l->n_pessoas;
}

int pessoa_entra(local_t *l, int id_pessoa){
    if (local_cheio(l)){
        return 0;
    }
    l->pessoas[l->n_pessoas] = id_pessoa;
    l->n_pessoas++;
    return 1;
}

int pessoa_sai(local_t *l, int id_pessoa){
    if(l->n_pessoas == 0){
        return 0;
    }

    int i,j,temp;
    /* Acha pessoa */
    for(i = 0; i < l->n_pessoas;i++){
        /* Se achar remove */
        if(l->pessoas[i] == id_pessoa){
            for(j = i; j < l->n_pessoas-1; j++){
                temp =l->pessoas[i];
	            l->pessoas[i]=l->pessoas[i+1];
	            l->pessoas[i+1]=temp;
            }
            l->n_pessoas--;
            return 1;
        }
    }
    return 0;
}


int tempo_viagem(local_t *l1, local_t *l2, int p_idade){
    int idade_modf = 0;
    if (0 < p_idade-40){
        idade_modf = p_idade-40; 
    }
    return distancia(l1, l2) /(100 - idade_modf);
}

/********************Rumores********************/
/* Rumores são conjuntos com cada rumor sendo um int de valor unico
*  O conjunto de todos os rumores é feito em cria_mundo()
*  O conjunto de rumores de cada pessoa é criado pela função subconjunto() chamada em cria_pessoa
*/

/********************Mundo********************/

/* Espera que srand() ja tenha sido chamado */
mundo_t* cria_mundo(int n_pessoas, int n_locais, int n_rumores, int tam){
    
    mundo_t* m = malloc(sizeof(mundo_t));
    if (m == NULL){
        return NULL;    
    }

    /* Alocando array pessoas */
    m->pessoas = malloc(sizeof(pessoa_t*)*n_pessoas);
    if (m->pessoas == NULL){
        free(m);
        return NULL;
    }

    /* Alocando Array locais */
    m->locais = malloc(sizeof(local_t*)*n_locais);
    if (m->locais == NULL){
        free(m->pessoas);
        free(m);
        return NULL;
    }

    /* Variaveis Loops */
    int i,j;
    
    /* Alocando Rumores*/
    m->rumores = cria_conjunto(n_rumores);
    if (m->rumores == NULL){
        free(m->locais);
        free(m->pessoas);
        free(m);
        return NULL;
    }
    
    /*Criando rumores*/
    for (i = 0; i < n_rumores; i++){
        insere_conjunto(m->rumores,i);
    }
    
    /* Criando pessoas */
    for (i = 0; i < n_pessoas; i++){
        m->pessoas[i] = cria_pessoa(i,m->rumores);
        if (m->pessoas[i] == NULL){
            /* Destroi as pessoas que ja foram alocadas */
            for (j = i-1; j >= 0; j--){
                destroi_pessoa(m->pessoas[j]);
            }
            
            destroi_conjunto(m->rumores);
            free(m->locais);            
            free(m->pessoas);
            free(m);

            return NULL;
        }

    }

    /* Criando Locais */
    for (i = 0; i < n_locais; i++){
        m->locais[i] = cria_local(i,tam);
        if (m->locais[i] == NULL){
            
            /* Destroi os locais que ja foram alocadas */
            for (j = i-1; j >= 0; j--){
                destroi_local(m->locais[j]);
            }

            /*Destroi as pessoas alocadas*/
            for (j = 0; i < n_pessoas; i++){
                destroi_pessoa(m->pessoas[j]);
            }

            destroi_conjunto(m->rumores);
            free(m->locais);            
            free(m->pessoas);
            free(m);

            return NULL;
        }
    }

    m->n_tamanho_mundo = tam;
    m->n_pessoas       = n_pessoas;
    m->n_locais        = n_locais;
    m->tempo_atual     = 0;
    
    return m;
}

mundo_t* destroi_mundo(mundo_t* m){

    int i;
    /* Destroi os locais que ja foram alocadas */
    for (i = 0; i < m->n_locais; i++){
        destroi_local(m->locais[i]);
    }

    /*Destroi as pessoas alocadas*/
    for (i = 0; i < m->n_pessoas; i++){
        destroi_pessoa(m->pessoas[i]);
    }

    /* Apagando dados */
    m->n_tamanho_mundo = 0;
    m->n_pessoas       = 0;
    m->n_locais        = 0;
    m->tempo_atual     = 0;
    
    free(m->locais);            
    free(m->pessoas);
    destroi_conjunto(m->rumores);
    free(m);

    return NULL;    
}

int atualiza_tempo(mundo_t* m, int tempo){
    if (tempo < m->tempo_atual){
        printf("Erro: O nomo valor de tempo [%i] é inferior ao tempo atual %i\n", tempo, m->tempo_atual);
        return 0;
    }
    m->tempo_atual = tempo;
    return 1;
    
}