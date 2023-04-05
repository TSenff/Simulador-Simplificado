#ifndef EVENTOS_H_
#define EVENTOS_H_

#include "liblef.h"
#include "entidades.h"

/**********************Rumores************************/

struct evento_rumores{
    int id_pessoa;
    int id_local;
    conjunto_t *rumores;
};
typedef struct evento_rumores e_rumores;

/* Cria um evento de disseminação, retorna NULL caso falha */
evento_t* cria_disseminacao(int tempo, pessoa_t *pessoa, int id_local);

/* Destroi um evento de dissemionação */
evento_t* destroi_disseminacao(evento_t *e);

/**********************Chegada************************/

struct  evento_chegada{
    int id_pessoa;
    int id_local;
};
typedef struct evento_chegada e_chegada;

/* Cria um evento de chegada, retorna NULL caso falha */
evento_t* cria_chegada(int tempo, int id_pessoa, int id_local);

/* Destroi um evento de chegada */
evento_t* destroi_chegada(evento_t *e);

/**********************Saida************************/

struct evento_saida{
    int id_pessoa;
    int id_local;
    
};
typedef struct evento_saida e_saida; 

/* Cria um evento de saida, retorna NULL caso falha */
evento_t* cria_saida(int tempo, int id_pessoa, int id_local);

/* Destroi um evento de saida */
evento_t* destroi_saida(evento_t *e);


/**********************Fim************************/

/* Cria um evento de Fim */
evento_t* cria_fim(int tempo);

/* Destroi um evento de Fim */
evento_t* destroi_fim(evento_t *e);

/**********************Misc************************/

/* Trata um evento de acordo com seu tipo. Espera que srand() tenha sido chamada
*  retorna -1 caso o evento tratado seja do tipo FIM 
*  retorna 0 caso algum erro ocorra
*  retorna 1 caso o evento seja tratado com exito
*/
int trata_evento(evento_t *e, mundo_t *mundo, lef_t *l);

/*
* Inicia um evento de chegada na lef(l) para cada pessoa em mundo(m), retorna 1 se inicializado, 0 caso falha
*/
int inicializa_eventos_mundo(mundo_t *m, lef_t *l);

#endif