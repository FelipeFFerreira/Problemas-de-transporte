
/* listaord.c
 *
 * Implementação das operações sobre o TAD lista ordenada implementada
 * de forma encadeada.
 *
 * Felipe Ferreira
 *
 */

#include "listaord.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void lst_init(lst_ptr * l) {
    *l = NULL;
}

void lst_ins(lst_ptr * l, lst_info val) {

   if(lst_find(*l, val.name) == NULL) {
        lst_ptr n;
        if ((n = (lst_ptr) malloc(sizeof(struct lst_no))) == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria!\n");
            exit(1);
        }
        n->dado = val;
        if (*l == NULL || strcmp(val.name, (*l)->dado.name) <= 0){
        n->prox = *l;
        *l = n;
        return;
        }
        else {
            lst_ptr p = *l;
            while (p->prox != NULL && strcmp(val.name,  p->prox->dado.name) > 0) {
                p = p->prox;
            }
            n->prox = p->prox;
            p->prox = n;
            return;
        }
    }

}


lst_ptr lst_find(lst_ptr l, char name[21] ) {

    while(l != NULL && strcmp(name, l->dado.name) > 0) {
        l = l->prox;
    }
    if (l != NULL && strcmp(name, l->dado.name) == 0) {
        return l;
    }
    return NULL;
}

void lst_print(lst_ptr l, FILE * fptr) {
    printf("[ ");
    fprintf(fptr, "[ ");
    while (l != NULL) {
        printf("%s ,", l->dado.name);
        fprintf(fptr, "%s ,", l->dado.name);
        l = l->prox;
    }
    printf("\b ]\n");
    fprintf(fptr, "]\n");
}
