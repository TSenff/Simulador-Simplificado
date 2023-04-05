#include "eventos.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

/**********************/
#define FIM 0
#define CHEGADA 1
#define SAIDA 2 
#define RUMOR 3
/**********************/




/********************Destroi dados********************/

/* Essa função é desnecessaria e existe apenas por padronização, (Caso no futuro seja adicionado dados a fim)*/
void destroi_dados_fim(void *ptr){

    /*Essa linha so existe para o valgrind não reclamar mas ela pode ser apagada
    * O valgrind reclama pois as funçoes da lef usam malloc para dados de todos os eventos 
    * inclusive os dados do evento FIM porem como tamanho de dados de fim é 0 esse malloc não faz nada
    * e por isso não precisam de free, mas o valgrind não detecta isso
    */
    free(ptr);
}

void destroi_dados_chegada(void *ptr){
    free(ptr);
}

void destroi_dados_saida(void *ptr){
    free(ptr);
}

void destroi_dados_disseminacao(void *ptr){
    destroi_conjunto(((e_rumores*)ptr)->rumores);
    free(ptr);

}

/********************Destroi eventos********************/

evento_t* destroi_fim(evento_t *e){
    destroi_dados_fim(e->dados);
    free(e);
    return NULL;
}

evento_t* destroi_chegada(evento_t *e){
    destroi_dados_chegada(e->dados);
    free(e);
    return NULL;
}

evento_t* destroi_saida(evento_t *e){
    destroi_dados_saida(e->dados);
    free(e);
    return NULL;
}

evento_t* destroi_disseminacao(evento_t *e){
    destroi_dados_disseminacao(e->dados);
    free(e);
    return NULL;
}

/********************Criando Eventos********************/

evento_t* cria_fim(int tempo){
    evento_t *fim = malloc(sizeof(evento_t));
    if (fim == NULL){
        return NULL;
    }

    fim->tempo   = tempo;
    fim->tipo    = 0; 
    fim->tamanho = 0;
    fim->dados   = NULL;
    fim->destroidados = destroi_dados_fim;
    return fim;
}

evento_t* cria_chegada(int tempo, int id_pessoa, int id_local){
    evento_t *chegada = malloc(sizeof(evento_t));
    if (chegada == NULL){
        return NULL;
    }
    
    chegada->dados = malloc(sizeof(e_chegada));
    if (chegada->dados == NULL){
        free(chegada);
        return NULL;
    }

    chegada->tempo = tempo;
    chegada->tipo  = 1; 
    chegada->tamanho = sizeof(e_chegada);

    e_chegada e;
    e.id_pessoa = id_pessoa;
    e.id_local  = id_local;


    
    memcpy(chegada->dados,&e,chegada->tamanho);
    
    chegada->destroidados = destroi_dados_chegada;

    return chegada;
}

evento_t* cria_saida(int tempo, int id_pessoa, int id_local){
    evento_t *saida = malloc(sizeof(evento_t));
    if (saida == NULL){
        return NULL;
    }

    saida->tempo = tempo;
    saida->tipo  = 2; 

    saida->tamanho = sizeof(e_chegada);

    /* Dados do evento Saida */
    e_saida e;
    e.id_pessoa = id_pessoa;
    e.id_local  = id_local;

    saida->dados   = malloc(sizeof(e_saida));
    memcpy(saida->dados,&e,saida->tamanho);
    
    saida->destroidados = destroi_dados_saida;

    return saida;
}

evento_t* cria_disseminacao(int tempo, pessoa_t *pessoa, int id_local){
    evento_t *disseminacao = malloc(sizeof(evento_t));
    if (disseminacao == NULL){
        return NULL;
    }

    disseminacao->tempo = tempo;
    disseminacao->tipo  = 3; 
    disseminacao->tamanho = sizeof(e_rumores);
    
    /* Dados do evento disseminação */
    e_rumores e;
    e.id_pessoa = pessoa->id;
    e.id_local  = id_local;
    e.rumores   = cria_subconjunto(pessoa->rumores, (rand() % pessoa->rumores->card)+1);
    if (e.rumores == NULL){
        return NULL;
    }
    disseminacao->dados   =  malloc(disseminacao->tamanho);
    if (disseminacao->dados == NULL){
        return NULL;
    }
    memcpy(disseminacao->dados,&e,disseminacao->tamanho);
    
    disseminacao->destroidados = destroi_dados_disseminacao;

    return disseminacao;
}

/*****************Inicializa*****************/

int inicializa_eventos_mundo(mundo_t *m, lef_t *l){
    evento_t *e;
    int i;
    for (i = 0; i < m->n_pessoas; i++){

        /* Local_destino */
        int id_local_destino = rand() % m->n_locais;
        /*Inicializa a pessoa em um local aleatorio*/
        int tempo = tempo_viagem(m->locais[rand() % m->n_locais], m->locais[id_local_destino],m->pessoas[i]->idade);
        e = cria_chegada((m->tempo_atual + tempo/15),m->pessoas[i]->id, id_local_destino);
        if (e == NULL){
            return 0;
        }
        adiciona_ordem_lef(l, e);
        destroi_chegada(e);
    }
    return 1;
}

/*****************Trata Eventos*****************/

int trata_evento(evento_t* e, mundo_t* m, lef_t *l){
    /*Variavel usada para criar eventos futuros*/
    evento_t *prox_evento;
    
    switch (e->tipo){
    /* Fim do Mundo */
    case FIM:
        printf("%5i : FIM DO MUNDO\n",m->tempo_atual);
        destroi_mundo(m);
        destroi_lef(l);
        destroi_fim(e);        
        return -1;
        break;
    /* Chegada */
    case CHEGADA:{
        /* Processando dados do evento */
        e_chegada *dados_c= (e_chegada*)e->dados;

        /* Mensagem de Chegada  (Continua abaixo)*/
        printf("%5i : CHEGA Pessoa  %2i  Local  %2i ( %2i / %2i ),",m->tempo_atual,dados_c->id_pessoa,dados_c->id_local,m->locais[dados_c->id_local]->n_pessoas,m->locais[dados_c->id_local]->max);

        /* Caso local lotado */
        if (local_cheio(m->locais[dados_c->id_local])){
            /*Se o tamanho da fila foi maior que paciencia/4 vai para outro lugar */
            if (m->pessoas[dados_c->id_pessoa]->pac/4 > m->locais[dados_c->id_local]->fila->tamanho){
                insere_fila(m->locais[dados_c->id_local]->fila, dados_c->id_pessoa);
                /* Mensagem entrou na fila do local*/
                printf(" FILA %i",m->locais[dados_c->id_local]->fila->tamanho);               

            }
            else{

                /* Mensagem desitiu*/
                printf(" DESISTE");

                prox_evento = cria_saida(m->tempo_atual,dados_c->id_pessoa, dados_c->id_local);
                if (prox_evento == NULL){
                    printf("Erro: falha ao criar evento diseminação\n");
                    return 0;
                }
                adiciona_ordem_lef(l, prox_evento);
                destroi_saida(prox_evento);

            } 
            /* Fim mensagem Chegada caso local cheio*/
            printf("\n");
            /* Liberando Evento */
            destroi_chegada(e);
            return 1;
        }
        /* Mensagem entrou no local (E fim mensagem de chegada se local !Cheio)*/
        printf(" ENTRA\n");
        pessoa_entra(m->locais[dados_c->id_local], dados_c->id_pessoa);

        /* Tempo de permanencia */  
        int n_rand = (rand()% 9)-2;
        int tpl = (0 >= (m->pessoas[dados_c->id_pessoa]->pac/10 + n_rand)? 1 : (m->pessoas[dados_c->id_pessoa]->pac/10 + n_rand));
        
        /* Criando Saida */
        prox_evento = cria_saida(m->tempo_atual+tpl,dados_c->id_pessoa, dados_c->id_local);
        if (prox_evento == NULL){
            printf("Erro: falha ao criar evento Saida\n");
            return 0;
        }
        adiciona_ordem_lef(l, prox_evento);
        destroi_saida(prox_evento);

        /* Criando Disseminação */
        prox_evento = cria_disseminacao(m->tempo_atual+(rand() % tpl),m->pessoas[dados_c->id_pessoa], dados_c->id_local);
        if (prox_evento == NULL){
            printf("Erro: falha ao criar evento diseminação\n");
            return 0;
        }   
        adiciona_ordem_lef(l, prox_evento);
        
        free(prox_evento->dados);
        free(prox_evento);

        destroi_chegada(e);
        break;
    }
    /* Saida */
    case SAIDA:{
        /* Processando dados do evento */
        e_saida *dados_s = (e_saida*)e->dados;
       
        /*Mensagem de saida (Continua a baixo)*/
        printf("%5i : SAIDA Pessoa  %2i  Local  %2i ( %2i / %2i )",m->tempo_atual,dados_s->id_pessoa,dados_s->id_local,m->locais[dados_s->id_local]->n_pessoas,m->locais[dados_s->id_local]->max);
        
        pessoa_sai(m->locais[dados_s->id_local], dados_s->id_pessoa);
       
        /* Verifica fila do local */
        if (!fila_vazia(m->locais[dados_s->id_local]->fila) && !local_cheio(m->locais[dados_s->id_local])){
            int id_pessoa_fila;
            retira_fila(m->locais[dados_s->id_local]->fila, &id_pessoa_fila);
            
            prox_evento = cria_chegada(m->tempo_atual, id_pessoa_fila, dados_s->id_local);
            if (prox_evento == NULL){
                printf("Erro: falha ao criar evento Chegada\n");
                return 0;
            }

            adiciona_inicio_lef(l,prox_evento);
            destroi_chegada(prox_evento);

            /* Mensagem de Pessoa removida da Fila*/
            printf(", REMOVE FILA Pessoa %2i",id_pessoa_fila);

        }

        /* Fim de mensagem de saida*/
        printf("\n");

        /* Cria novo evento de chegada */
        /* Local_destino */
        int id_local_destino = rand() % m->n_locais;

        /* Calcula Tempo de Viajem */
        int tempo = tempo_viagem(m->locais[dados_s->id_local], m->locais[id_local_destino], m->pessoas[dados_s->id_pessoa]->idade);
    
        /* Cria chegada em TEMPO = ATUAL +TEMPO_VIAGEM/15*/
        prox_evento = cria_chegada((m->tempo_atual + tempo/15),dados_s->id_pessoa, id_local_destino);
        if (prox_evento == NULL){
            printf("Erro: falha ao criar evento chegada\n");
            return 0;
        }
        
        adiciona_ordem_lef(l, prox_evento);
        destroi_chegada(prox_evento);

        destroi_saida(e);
        break;
    }
    /* RUMOR */
    case RUMOR:{

        /* Processando dados do evento */
        e_rumores *dados_r = (e_rumores*)e->dados;     

        /*Mensagem de rumor (Continua a baixo)*/
        printf("%5i : RUMOR Pessoa  %2i  Local  %2i ",m->tempo_atual,dados_r->id_pessoa,dados_r->id_local);

        iniciar_iterador(dados_r->rumores);

        /* Expalhando rumores para pessoas no local */
        int i,r_atual, p_atual;
        while (incrementar_iterador(dados_r->rumores,&r_atual)){  
            for (i = 0; i < m->locais[dados_r->id_local]->n_pessoas; i++){
                /* Verifica extroversão */
                p_atual = m->locais[dados_r->id_local]->pessoas[i];
                if (m->pessoas[p_atual]->extro > rand() % 101){
                    
                    /* Tenta inserir no conjunto */
                    int res_inser = insere_conjunto(m->pessoas[p_atual]->rumores,r_atual);
                    
                    /*Caso falte espaco*/
                    if(res_inser == -1){
                        /* Redimensiona e tenta inserir */
                        if (!redimensiona(m->pessoas[p_atual]->rumores))
                            return 0;
                        res_inser = insere_conjunto(m->pessoas[p_atual]->rumores,r_atual);
                    }
                    /*Se inserido informa*/
                    if(res_inser){
                        /* Mensagem Rumor aceito */
                        printf("(P%-2i : R%i)",p_atual,r_atual);
                    }
                }
            }
        }

        /* Fim Mensagem */
        printf("\n");

        destroi_disseminacao(e);
        break;
    }
    /* O evento não é reconhecido */
    default:
        return 0;
        break;
    }
    return 1;
}
