#ifndef _TRANSPORTE_H
#define _TRANSPORTE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "listaord.h"

#define MAX 15
#define overflow 150 //considerando sistema de 32 bits

typedef struct {
    char name[21];  //indentificador Nome
    lst_ptr destinos; //lista para lista de nós destinos
    int ofertaDemanda; //oferta ou demanda
    int ofertaDemandaCustomizada;
    bool flagTransbordo; //flag de transbordo
    bool flagAdrresExistente; // flag de confirmação de no ja allocado
    int divisaoCustos;
}infoNo;

typedef struct lstNo * ptrNo;

 struct lstNo {
    infoNo dado;
    ptrNo prox;
};

void mallocNos(ptrNo * , ptrNo * );

void reallocListMatriz(ptrNo matriz, int qtdOrigens, int qtdDestinos);

void mallocPtrList(lst_ptr * p);

void inicializar_No(ptrNo *);

ptrNo inserir_No(ptrNo *, char *);

ptrNo lst_findNo(ptrNo, char *);

void imprimir_Nos(ptrNo, FILE *);

int sizeListNos(ptrNo l, int *);

void vereficaTransbordos(ptrNo origens, ptrNo destinos);

void ofertaDemanda(ptrNo l , char * strOp );

void MatrizTransporteInit(ptrNo origens, ptrNo destinos, FILE *);

void castMatrizTransporteInit(lst_info matrizCusto[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestino, int lin, int col);

void minimos(lst_info matriz[][MAX], int lin, int col, ptrNo * vetorOrigens, ptrNo * vetorDestinos, lst_info *);

void celulaInfoMatrizInit(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int lin, int col, int totalOferta, int totalDemanda);

void imprimirMatrizTransporte(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int op);

void imprimirMatrizTransporteDiviCustos(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int lin, int col);

void vetoresOrigensDestinosInit(ptrNo origens, ptrNo destinos, ptrNo * vetorOrigens, ptrNo * vetorDestino);

void insercao (lst_info * v, int tam);

void metodoMenorCusto(lst_info matrizCusto[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, lst_info * vetMinimos, int qtdOrigens, int qtdDestinos);

void divisaoCustos(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int qtdOrigens, int qtdDestinos);

void analiseEconomica(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int qtdOrigens, int qtdDestinos);

void maiorMatriz(lst_info matriz[][MAX], int lin, int col, int * idLin, int * idCol);

void roteamentoMatriz(lst_info matriz[][MAX], int lin, int col, int idlin, int idCol);

void reiniciaDivisaoCustos(ptrNo * vetorOrigens, ptrNo * vetorDestinos, int qtdOrigens, int qtdDestinos);

void reiniciaAnaliseEconomica(lst_info matriz[][MAX], int lin, int col);

int calculoTransporte(lst_info matriz[][MAX], int lin, int col);

#endif
