#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"

conjunto_t * cria_conjunto(int max){
    conjunto_t *c = malloc(sizeof(conjunto_t));
    if(c == NULL)
        return NULL;
    
    c->v = malloc(sizeof(int)*max);
    if(c->v == NULL){
        free(c);
        return NULL;
    }   

    c->max  = max;
    c->card = 0;
    c->ptr = -1;
    
    return c;
}

conjunto_t *destroi_conjunto (conjunto_t *c){;
    free(c->v);
    free(c);
    
    return NULL;
}

int conjunto_vazio (conjunto_t *c){
    return cardinalidade(c) == 0;
}

int cardinalidade (conjunto_t *c){
    return c->card;
}

/***************************** Funções Auxiliares   ****************************/

/* Insere um elemento no conjunto sem ordenacao ou verificação de repetição, verifica apenas se cheia */
int insere_conjunto_livre(conjunto_t *c, int elemento){

    /*Conjunto cheio*/
    if(cardinalidade(c) == c->max)
        return -1;

    c->v[cardinalidade(c)] = elemento;
    c->card++;
    return 1;
}

/*
 * Troca a posição de dois elementos de um vetor
 */
void troca(int v[], int a, int b){
	int x = v[a];
	v[a]=v[b];
	v[b]=x;
}

/*
* Busca em um vetor ordenado o index que um valor teria caso ele estivesse no vetor
*/
int busca_posicao(int vetor[], int tam, int valor){
	/* Caso todos os elementos ja tenham sido comparados */
	if(0 > tam-1) return -1;
    /* Continua a busca */
	if(vetor[tam-1] > valor) return busca_posicao(vetor, tam-1, valor); 
    /* Posição encontrada */
    return tam-1;
}

int busca_binaria(int vetor[], int ini, int fim, int valor){
	
	/*Caso todos os valores ja tenham sido comparados*/
	if (fim < ini)
        return -1;
	
    /*Encontrando meio do vetor*/
    int meio = ini + (fim-ini)/2;
	
	/*Caso o valor seja encontrado*/
    if(vetor[meio] == valor)
        return meio;
    
    /*Caso o valor atual seja maior que o procurado*/
    if(vetor[meio] > valor)
        return busca_binaria(vetor, ini, meio-1, valor);

	/*Caso o valor atual seja menor que o procurado*/
    return busca_binaria(vetor,meio+1, fim ,valor);

}

/***************************************************************************************/

int insere_conjunto (conjunto_t *c, int elemento){
    /*Conjunto cheio*/
    if(cardinalidade(c) == c->max)
        return -1;

    /*Insere vazio*/
    if(conjunto_vazio(c)){
        c->v[0] = elemento;
        c->card++;
        return 1;
    }

    /*Verifica se elemento existente*/
    if(busca_binaria(c->v,0 , cardinalidade(c)-1, elemento) != -1)
        return 0;
    
    /*Insere ordenado*/
    c->v[cardinalidade(c)] = elemento;
	int pos = busca_posicao(c->v, cardinalidade(c), c->v[cardinalidade(c)]);
	int i = cardinalidade(c);
	while(i>pos+1){
		troca(c->v, i, i-1);
		i--;
	}
    
    c->card++;

    return 1;

}

int retira_conjunto (conjunto_t *c, int elemento){

    /* busca_binaria faz a verificação de cardinalidade == 0 */
    int pos = busca_binaria(c->v, 0, cardinalidade(c)-1, elemento);
    if (pos == -1){
        return 0;
    }

    /*Deletando dados*/
    c->v[pos] = 0;

    /*Movendo elementos do vetor*/
    int i;
    for(i=pos; i <= cardinalidade(c)-2; i++){
        troca(c->v, i,i+1);
    }

    c->card--;

    return 1;
}

int pertence (conjunto_t *c, int elemento){
    return busca_binaria(c->v,0,cardinalidade(c)-1,elemento) != -1;
}

int contido (conjunto_t *c1, conjunto_t *c2){
    int i;
    for(i = 0; i < c1->card;i++){
        if(!pertence(c2,c1->v[i])){
            return 0;
        }
    }
    return 1;
}

/*
 * Retorna 1 se o conjunto c1 eh igual ao conjunto c2 e 0 caso contrario.
 */
int sao_iguais (conjunto_t *c1, conjunto_t *c2){
    
    /*Verifica cardinalidade*/
    if(cardinalidade(c1) != cardinalidade(c2)){
        return 0;
    }

    /*Verifica Elementos*/
    int i;
    for(i = 0; i < c1->card;i++){
        if(c1->v[i] != c2->v[i]){
            return 0;
        }
    }
    return 1;
}


conjunto_t * cria_diferenca (conjunto_t *c1, conjunto_t *c2){

    /*Verifica diferença de conjunto vazio*/
    if(conjunto_vazio(c2)){
        return cria_copia(c1);
    }

    conjunto_t *inter = cria_conjunto(cardinalidade(c1));

    /*Adiciona em dif os elementos de c1 que não pertencem a c2*/
    int i;
    for(i = 0; i < c1->card; i++){
        if (!pertence(c2, c1->v[i])){
            insere_conjunto_livre(inter, c1->v[i]);
        }
    }

    return inter;
    

}

conjunto_t * cria_interseccao (conjunto_t *c1, conjunto_t *c2){
    
    /*Verifica conjunto vazio*/
    if(conjunto_vazio(c1) || conjunto_vazio(c2)){
        /*Conjunto vazio manda tamanho 0 mesmo?*/
        return cria_conjunto(1);
    }
    
    /*O tamanho do novo conjunto é igual ao menor conjunto*/
    conjunto_t *inter;
    if (cardinalidade(c1) < cardinalidade(c2)){
        inter = cria_conjunto(cardinalidade(c2));
    }
    else{
        inter = cria_conjunto(cardinalidade(c1));
    }

    if (inter == NULL){
        return 0;
    }
    
    /* Verifica se um item pertence a c1 e c2 e adiciona em inter*/
    int i = 0;
    for(i = 0; i < c1->card; i++){
        if (pertence(c2, c1->v[i])){
            insere_conjunto_livre(inter, c1->v[i]);
        }
    }
    
    return inter;
}


conjunto_t * cria_uniao (conjunto_t *c1, conjunto_t *c2){

    conjunto_t *uniao = cria_conjunto(cardinalidade(c1)+cardinalidade(c2));
    int i;
    for(i = 0; i < c1->card; i++){
        insere_conjunto_livre(uniao, c1->v[i]);
    }
    for(i = 0; i < c2->card; i++){
        insere_conjunto(uniao, c2->v[i]);
    }
    return uniao;

}

conjunto_t * cria_copia (conjunto_t *c){
    conjunto_t *copia = cria_conjunto(c->max);
    if(copia == NULL)
        return 0;

    int i;
    for(i = 0; i < c->card; i++){
        insere_conjunto_livre(copia,c->v[i]);
    }

    return copia;
}


conjunto_t * cria_subconjunto (conjunto_t *c, int n){

    conjunto_t *sub = cria_copia(c);
    int pos;

    int i,j;
    for (i = cardinalidade(sub); i > n; i--){
        
        /* Elemento a resolver */
        pos = rand() % cardinalidade(sub);
        
        /* Movendo elementos do vetor */
        for(j=pos; j <= cardinalidade(sub)-2; j++){
            troca(sub->v, j, j+1);
        }

        sub->card--;
                
    }
    return sub;    
}


void imprime (conjunto_t *c){
    int i;
    for(i = 0; i < c->card;i++){
        printf("%d ",c->v[i]);
    }
    printf("\n");
}


int redimensiona (conjunto_t *c){
    c->v = realloc(c->v,sizeof(int)*c->max*2);
    if(c->v == NULL){
        return 0;
    }
    c->max = c->max*2;
    return 1;
}

/*
 * As funcoes abaixo permitem acessar elementos apontados pelo ponteiro 'ptr'.
 * Este ponteiro pode ser inicializado e incrementado, viabilizando
 * a implementacao de um mecanismo iterador.
 */

void iniciar_iterador (conjunto_t *c){
    c->ptr = -1;
}


int incrementar_iterador (conjunto_t *c, int *elemento){
    c->ptr++;
    if(c->ptr >= cardinalidade(c)){
        c->ptr--;
        return 0;
    }
    *elemento = c->v[c->ptr];
    return 1;
}

int retirar_um_elemento (conjunto_t *c){

    c->card--;

    /*Deletando dados*/
    int elem = c->v[cardinalidade(c)];
    c->v[cardinalidade(c)] = 0;

    return elem;
}
