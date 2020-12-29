/* listaord.h
 *
 * Definição do TAD para representar uma lista ordenada implementada
 * de forma encadeada e protótipo das operações sobre esse TAD.
 *
 * Felipe Ferreira
 *
 */

#ifndef _LISTAORD_H
#define _LISTAORD_H
#include <stdbool.h>
#include <stdio.h>

typedef struct lstInfo lst_info;

typedef struct {

	int lin;
	int col;
}matrizCordenadas;

struct lstInfo {

    char name[21];
    void * adressDestino;
    int custo; //custo Original
    int custoOtimizado; // para otimização dos custos
    int analiseEconomica; //para analise economica
    matrizCordenadas cordenadas; // cordenadas na matriz
};

typedef struct lst_no * lst_ptr;
struct lst_no {
    lst_info dado;
    lst_ptr prox;
};

/* inicializa a lista ordenada */
void lst_init(lst_ptr *);

/* insere um novo elemento na lista ordenada */
void lst_ins(lst_ptr *, lst_info);

/* remove um elemento da lista ordenada */
bool lst_rem(lst_ptr *, lst_info);

/* procura e devolve o ponteiro para o nó da lista
 * que contém um valor, ou NULL, caso o valor não
 * seja localizado */
lst_ptr lst_find(lst_ptr, char *);

/* imprime os elementos da lista ordenada */
void lst_print(lst_ptr, FILE *);

#endif

