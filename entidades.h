#ifndef ENTIDADE_H_
#define ENTIDADE_H_

#include "libconjunto.h"
#include "libfila.h"
#include <math.h>

/********************Pessoa********************/

typedef struct pessoa{
    int id;
    int extro;
    int pac;
    int idade;
    conjunto_t *rumores;

}pessoa_t;

/* Cria uma pessoa, retorna NULL caso ocorra um erro
*  Espera que srand() ja tenha sido chamado 
*/
pessoa_t* cria_pessoa(int id, conjunto_t *rumores_mundo);
/* Destroi pessoa */
pessoa_t* destroi_pessoa(pessoa_t *p);



/********************Local********************/

/* Cordenadas de um local*/
typedef struct cord{
    int x;
    int y;
}cord_t;

typedef struct local{
    int id;
    int max;
    int n_pessoas;
    int* pessoas;
    fila_t *fila;
    cord_t *pos;
}local_t;

/* Cria um Local, retorna NULL caso ocorra um erro
*  Espera que srand() ja tenha sido chamado 
*/
local_t* cria_local(int id, int tam_mundo);

/* Destroi Local */
local_t* destroi_local(local_t *l);

/* Calcula o tempo de viagem entre dois locais 
*  Usa a idade da pessoa viajando para modificar o tempo
*/
int tempo_viagem(local_t *l1, local_t *l2, int p_idade);

/* Retorna 1 se o local esta chieo, 0 caso não*/
int local_cheio(local_t *l);

/* Adiciona o id de uma pessoa entrando no local */
int pessoa_entra(local_t *l, int id_pessoa);

/* Remove o id de uma pessoa saido do local*/
int pessoa_sai(local_t *l, int id_pessoa);

/********************Rumores********************/
/* Rumores são conjuntos com cada rumor sendo um int de valor unico
*  O conjunto de todos os rumores é feito em cria_mundo()
*  O conjunto de rumores de cada pessoa é criado pela função subconjunto() chamada em cria_pessoa
*/

/********************Mundo********************/
typedef struct mundo{
    int tempo_atual;
    pessoa_t **pessoas;
    local_t  **locais;
    conjunto_t *rumores;
    int n_pessoas;
    int n_locais;
    int n_tamanho_mundo;
}mundo_t;

/* Cria um Mundo, retorna NULL caso ocorra um erro
*  Espera que srand() ja tenha sido chamado 
*/
mundo_t* cria_mundo(int n_pessoas, int n_locais, int n_rumores, int tam);
/* Destroi Mundo */
mundo_t* destroi_mundo(mundo_t* m);

/* Atualiza o tempo do mundo, caso o novo tempo seja menor que o anterior retorna 0*/
int atualiza_tempo(mundo_t* m, int tempo);

#endif
