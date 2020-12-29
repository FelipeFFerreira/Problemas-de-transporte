#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "transporte.h"
#include "listaord.h"

void mallocNos(ptrNo * origens, ptrNo * destinos, FILE * fptr)
{
    char origem[21], destino[21], op;
    int custo;
    lst_info info_list;

    do {
        printf("\n");
        printf(">Origem: ");
        scanf(" %20[^\n]", origem);
        printf(">Destino: ");
        scanf(" %20[^\n]", destino);
        printf(">Custo: ");
        scanf("%d", &custo);
        printf("\n");

        /*Se possivel, aloca e Recebe endereço dos respecitos nos, origem e destino*/
        ptrNo ptrOrigem = inserir_No(origens, origem);
        ptrNo ptrDestino = inserir_No(destinos, destino);
        /*criar e inicializa ponteiro para lista de destinos*/
        //mallocPtrList(&ptr_list_destinos);
        if (!ptrOrigem->dado.flagAdrresExistente){
            if ((ptrOrigem->dado.destinos = (lst_ptr) malloc(sizeof(lst_ptr))) == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria <0x1>ptrOrigem!\n");
            exit(1);
            }
            lst_init(&ptrOrigem->dado.destinos);
            info_list.adressDestino = (void *)ptrDestino;
            ptrOrigem->dado.flagAdrresExistente = true;
        }

        info_list.custo = custo;
        strcpy(info_list.name, destino);
        lst_ins(&ptrOrigem->dado.destinos, info_list);

        printf(">Mais Rotas: S/N: ");
        scanf(" %c", &op);
    } while (op != 'n');

    vereficaTransbordos(*origens, *destinos);
    ofertaDemanda(*origens , "oferta");
    printf("\n\n");
    ofertaDemanda(*destinos, "demanda");
    //printf("\n&%p cont %p\n", origens, *origens);
    //MatrizTransporte(origem, destino);
}

void MatrizTransporteInit(ptrNo origens, ptrNo destinos, FILE * fptr)
{
    int totalDemanda, totalOferta, totalOfertaDemanda, qtdOrigens, qtdDestinos, idLin, idCol;
    char outros[] = "Outros", oferta[] = "Oferta", demanda[] = "Demanda", origem[] = "Origem";

    qtdOrigens = sizeListNos(origens, &totalOfertaDemanda);
    totalOferta = totalOfertaDemanda;
    qtdDestinos = sizeListNos(destinos, &totalOfertaDemanda);
    totalDemanda = totalOfertaDemanda;

    if (totalOferta == totalDemanda){
    	printf("\n\n>[ **** Foi Detectado Oferta == Demanda ****\n\n");
    	fprintf(fptr, "\n\n>[ **** Foi Detectado Oferta == Demanda ****\n\n");
    	int numeroInteracoes = 0;
      lst_info matrizCusto[qtdOrigens + 2][MAX];
      ptrNo vetorOrigens[qtdOrigens + 3];
      ptrNo vetorDestinos[qtdDestinos + 3];
      vetorOrigens[0] = (ptrNo)&origem[0];
      vetorDestinos[0] = (ptrNo)&origem[0];
      vetorOrigens[qtdOrigens + 1] = (ptrNo)&demanda[0];
      vetorDestinos[qtdDestinos + 1] = (ptrNo)&oferta[0];
      vetorOrigens[qtdOrigens + 2] = NULL;
      vetorDestinos[qtdDestinos + 2] = NULL;

      vetoresOrigensDestinosInit(origens, destinos, vetorOrigens, vetorDestinos);
      celulaInfoMatrizInit(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos, totalOferta, totalDemanda);
      castMatrizTransporteInit(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos);
     	printf("\n\n ********** Quadro de Custos ***********\n\n");
     	fprintf(fptr, "\n\n ********** Quadro de Custos ***********\n\n");
     	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 0, fptr);
     	lst_info vetMinimos[qtdOrigens * qtdDestinos];
      minimos(matrizCusto, qtdOrigens, qtdDestinos, vetorOrigens, vetorDestinos, vetMinimos);
	 	  printf("\n\n ********** Quadro Inicial ***********\n\n");
	 	  fprintf(fptr, "\n\n ********** Quadro Inicial ***********\n\n");
	   	metodoMenorCusto(matrizCusto, vetorOrigens, vetorDestinos, vetMinimos, qtdOrigens, qtdDestinos);
	    imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr);
	 	  printf(">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos));
	 	  fprintf(fptr, ">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos));
	 	while(1) {
	 	numeroInteracoes += 1;
        printf("\n\n>[_______________________INTERACAO___________________________] [%d]\n\n", numeroInteracoes);
        fprintf(fptr, "\n\n>[_______________________INTERACAO___________________________] [%d]\n\n", numeroInteracoes);
        reiniciaDivisaoCustos(vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos);
        reiniciaAnaliseEconomica(matrizCusto, qtdOrigens, qtdDestinos);

   			divisaoCustos(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos);
   			analiseEconomica(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos);

   			imprimirMatrizTransporteDiviCustos(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos, fptr);
   			imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 2, fptr); //Imprimi analise

        maiorMatriz(matrizCusto, qtdOrigens, qtdDestinos, &idLin, &idCol);
        printf("\nPosicao K = matriz[%d][%d]\n", idLin, idCol);
        fprintf(fptr, "\nPosicao K = matriz[%d][%d]\n", idLin, idCol);
        if(matrizCusto[idLin][idCol].analiseEconomica < 0 || matrizCusto[idLin][idCol].analiseEconomica == 0) {
        	printf("\n\n>Todos os Valores na Analise encomica Enontra-se Otimizado (negativo)\n\n");
        	fprintf(fptr, "\n\n>Todos os Valores na Analise encomica Enontra-se Otimizado (negativo)\n\n");
        	break;
        }
      	roteamentoMatriz(matrizCusto, qtdOrigens, qtdDestinos, idLin, idCol);

      	printf("\n\n ********** Quadro Apos Roteamento ***********\n\n");
      	fprintf(fptr, "\n\n ********** Quadro Apos Roteamento ***********\n\n");
        imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr); //custosOtimzado
        printf(">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos));
        fprintf(fptr,">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos));
        printf("\n_____________________________________________________________\n");
  		fprintf(fptr,"\n_____________________________________________________________\n");
  	}

    printf("\n\n ********** Quadro de Custos ***********\n\n");
    fprintf(fptr, "\n\n ********** Quadro de Custos ***********\n\n");
    imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 0, fptr);
    printf("\n\n************ Quadro Final ************\n\n");
   	fprintf(fptr, "\n\n************ Quadro Final ************\n\n");
    imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr); //custosOtimzado
    printf(">Custo do Transporte = %d\n\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos));
   	fprintf(fptr,">Custo do Transporte = %d\n\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos));
   }

   if(totalOferta > totalDemanda){
   		printf("\n\n>[ **** Foi Detectado Oferta > Demanda ****\n\n");
   		fprintf(fptr, "\n\n>[ **** Foi Detectado Oferta > Demanda ****]\n\n");
   		int i, numeroInteracoes = 0;
        lst_info matrizCusto[qtdOrigens + 2][MAX];
        ptrNo vetorOrigens[qtdOrigens + 3];
        ptrNo vetorDestinos[qtdDestinos + 4];
        vetorOrigens[0] = (ptrNo)&origem[0];
        vetorDestinos[0] = (ptrNo)&origem[0];
        vetorOrigens[qtdOrigens + 1] = (ptrNo)&demanda[0];
        if ((vetorDestinos[qtdDestinos + 1] = (ptrNo) malloc(sizeof(struct lstNo))) == NULL) {
          fprintf(stderr, "Erro de alocacao de memoria[[ptrNo, Oferta > Demanda]!\n");
          exit(1);
        }
        strcpy(vetorDestinos[qtdDestinos + 1]->dado.name, "outros");

        vetorDestinos[qtdDestinos + 2] = (ptrNo)&oferta[0];

        vetorOrigens[qtdOrigens + 2] = NULL;
        vetorDestinos[qtdDestinos + 3] = NULL;

		vetoresOrigensDestinosInit(origens, destinos, vetorOrigens, vetorDestinos);
       	celulaInfoMatrizInit(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos, totalOferta, totalDemanda);
       	castMatrizTransporteInit(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos);
       	printf("\n\n ********** Quadro de Custos ***********\n\n");
       	fprintf(fptr, "\n\n ********** Quadro de Custos ***********\n\n");
       	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 0, fptr);
       	lst_info vetMinimos[qtdOrigens * qtdDestinos];
	     	minimos(matrizCusto, qtdOrigens, qtdDestinos, vetorOrigens, vetorDestinos, vetMinimos);
	 	    printf("\n\n ********** Quadro Inicial ***********\n\n");
	 	    fprintf(fptr, "\n\n ********** Quadro Inicial ***********\n\n");
	     	metodoMenorCusto(matrizCusto, vetorOrigens, vetorDestinos, vetMinimos, qtdOrigens, qtdDestinos);
	     	printf(">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos + 1));
	     	fprintf(fptr, ">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos + 1));

        for (i = 0; i < qtdOrigens; i++) {
   			if (vetorOrigens[i + 1]->dado.ofertaDemandaCustomizada != 0) {
				matrizCusto[i + 1][qtdDestinos + 1].custoOtimizado =  vetorOrigens[i + 1]->dado.ofertaDemandaCustomizada;
				vetorOrigens[i + 1]->dado.ofertaDemandaCustomizada -= vetorOrigens[i + 1]->dado.ofertaDemandaCustomizada;
   			}
   		}
        imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr);

   		while(1) {
   			numeroInteracoes += 1;
        	printf("\n\n>[_______________________INTERACAO___________________________] [%d]\n\n", numeroInteracoes);
        	fprintf(fptr, "\n\n>[_______________________INTERACAO___________________________] [%d]\n\n", numeroInteracoes);
        	reiniciaDivisaoCustos(vetorOrigens, vetorDestinos, qtdOrigens + 1, qtdDestinos);
        	reiniciaAnaliseEconomica(matrizCusto, qtdOrigens, qtdDestinos + 1);

   		   	divisaoCustos(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos + 1);
   			  analiseEconomica(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos + 1);

   		   	imprimirMatrizTransporteDiviCustos(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos + 1, fptr);
   			  imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 2, fptr); //Imprimi analise

        	maiorMatriz(matrizCusto, qtdOrigens, qtdDestinos + 1, &idLin, &idCol);
        	printf("\nPosicao K = matriz[%d][%d]\n", idLin, idCol);
        	fprintf(fptr, "\nPosicao K = matriz[%d][%d]\n", idLin, idCol);
        	if (matrizCusto[idLin][idCol].analiseEconomica < 0 || matrizCusto[idLin][idCol].analiseEconomica == 0) {
        		printf("\n\n>Todos os Valores na Analise encomica Enontra-se Otimizado (negativo)\n\n");
        		fprintf(fptr, "\n\n>Todos os Valores na Analise encomica Enontra-se Otimizado (negativo)\n\n");
        		break;
        	}
      		roteamentoMatriz(matrizCusto, qtdOrigens, qtdDestinos + 1, idLin, idCol);

      		printf("\n\n ********** Quadro Apos Roteamento ***********\n\n");
      		fprintf(fptr, "\n\n ********** Quadro Apos Roteamento ***********\n\n");
        	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr); //custosOtimzado
        	printf(">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos + 1));
        	fprintf(fptr, ">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos + 1));
        	printf("\n_____________________________________________________________\n");
        	fprintf(fptr, "\n_____________________________________________________________\n");
       	}

        printf("\n\n ********** Quadro de Custos ***********\n\n");
        fprintf(fptr, "\n\n ********** Quadro de Custos ***********\n\n");
        imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 0, fptr);
       	printf("\n\n************ Quadro Final ************\n\n");
       	fprintf(fptr, "\n\n************ Quadro Final ************\n\n");
       	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr); //custosOtimzado
       	printf(">Custo do Transporte = %d\n\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos + 1));
       	fprintf(fptr, ">Custo do Transporte = %d\n\n", calculoTransporte(matrizCusto, qtdOrigens, qtdDestinos + 1));

   }

	if (totalOferta < totalDemanda) {
		printf("\n\n>[ **** Foi Detectado Oferta < Demanda ****\n\n");
		fprintf(fptr, "\n\n>[ **** Foi Detectado Oferta < Demanda ****\n\n");
		int i, numeroInteracoes = 0;
   		lst_info matrizCusto[qtdOrigens + 3][MAX];
       	ptrNo vetorOrigens[qtdOrigens + 4];
       	ptrNo vetorDestinos[qtdDestinos + 3];
       	vetorOrigens[0] = (ptrNo)&origem[0];
       	vetorDestinos[0] = (ptrNo)&origem[0];
       	vetorOrigens[qtdOrigens + 2] = (ptrNo)&demanda[0];
       	vetorDestinos[qtdDestinos + 1] = (ptrNo)&oferta[0];
       	if ((vetorOrigens[qtdOrigens + 1] = (ptrNo) malloc(sizeof(struct lstNo))) == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria[[ptrNo, Oferta < Demanda]!\n");
            exit(1);
        }
        strcpy(vetorOrigens[qtdOrigens + 1]->dado.name, "outros");
        vetorOrigens[qtdOrigens + 1]->dado.divisaoCustos = 0;
       	vetorOrigens[qtdOrigens + 3] = NULL;
       	vetorDestinos[qtdDestinos + 2] = NULL;

       	vetoresOrigensDestinosInit(origens, destinos, vetorOrigens, vetorDestinos);
       	celulaInfoMatrizInit(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos, totalOferta, totalDemanda);
       	castMatrizTransporteInit(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens, qtdDestinos);
       	printf("\n\n ********** Quadro de Custos ***********\n\n");
       	fprintf(fptr, "\n\n ********** Quadro de Custos ***********\n\n");
       	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 0, fptr);
       	lst_info vetMinimos[qtdOrigens * qtdDestinos];
	 	minimos(matrizCusto, qtdOrigens, qtdDestinos, vetorOrigens, vetorDestinos, vetMinimos);
	 	printf("\n\n ********** Quadro Inicial ***********\n\n");
	 	fprintf(fptr, "\n\n ********** Quadro Inicial ***********\n\n");
	 	metodoMenorCusto(matrizCusto, vetorOrigens, vetorDestinos, vetMinimos, qtdOrigens, qtdDestinos);
	 	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr);

   		for (i = 0; i < qtdDestinos; i++) {
   			if (vetorDestinos[i + 1]->dado.ofertaDemandaCustomizada != 0) {
				matrizCusto[qtdOrigens + 1][i + 1].custoOtimizado =  vetorDestinos[i + 1]->dado.ofertaDemandaCustomizada;
				vetorDestinos[i + 1]->dado.ofertaDemandaCustomizada -= vetorDestinos[i + 1]->dado.ofertaDemandaCustomizada;
   			}
   		}
        printf(">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens + 1, qtdDestinos));
        fprintf(fptr, ">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens + 1, qtdDestinos));

        while (1) {
        	numeroInteracoes += 1;
        	printf("\n\n>[_______________________INTERACAO___________________________] [%d]\n\n", numeroInteracoes);
        	fprintf(fptr, "\n\n>[_______________________INTERACAO___________________________] [%d]\n\n", numeroInteracoes);
        	reiniciaDivisaoCustos(vetorOrigens, vetorDestinos, qtdOrigens + 1, qtdDestinos);
        	reiniciaAnaliseEconomica(matrizCusto, qtdOrigens + 1, qtdDestinos);

   			divisaoCustos(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens + 1, qtdDestinos);
   			analiseEconomica(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens + 1, qtdDestinos);

   			imprimirMatrizTransporteDiviCustos(matrizCusto, vetorOrigens, vetorDestinos, qtdOrigens + 1, qtdDestinos, fptr);
   			imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 2, fptr); //Imprimi analise

        	maiorMatriz(matrizCusto, qtdOrigens + 1, qtdDestinos, &idLin, &idCol);
        	printf("\nPosicao K = matriz[%d][%d]\n", idLin, idCol);
        	fprintf(fptr, "\nPosicao K = matriz[%d][%d]\n", idLin, idCol);
        	if (matrizCusto[idLin][idCol].analiseEconomica < 0 || matrizCusto[idLin][idCol].analiseEconomica == 0) {
        		printf("\n\n>Todos os Valores na Analise encomica Enontra-se Otimizado (negativo)\n\n");
        		fprintf(fptr, "\n\n>Todos os Valores na Analise encomica Enontra-se Otimizado (negativo)\n\n");
        		break;
        	}
      		roteamentoMatriz(matrizCusto, qtdOrigens + 1, qtdDestinos, idLin, idCol);

      		printf("\n\n ********** Quadro Apos Roteamento ***********\n\n");
      		fprintf(fptr, "\n\n ********** Quadro Apos Roteamento ***********\n\n");
        	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr); //custosOtimzado
        	printf(">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens + 1, qtdDestinos));
        	fprintf(fptr, ">Custo do Transporte = %d\n", calculoTransporte(matrizCusto, qtdOrigens + 1, qtdDestinos));
        	printf("\n_____________________________________________________________\n");
        	fprintf(fptr, "\n_____________________________________________________________\n");
       	}

        printf("\n\n ********** Quadro de Custos ***********\n\n");
        fprintf(fptr, "\n\n ********** Quadro de Custos ***********\n\n");
        imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 0, fptr);
       	printf("\n\n************ Quadro Final ************\n\n");
       	fprintf(fptr, "\n\n************ Quadro Final ************\n\n");
       	imprimirMatrizTransporte(matrizCusto, vetorOrigens, vetorDestinos, 1, fptr); //custosOtimzado
       	printf(">Custo do Transporte = %d\n\n", calculoTransporte(matrizCusto, qtdOrigens + 1, qtdDestinos));
       	fprintf(fptr, ">Custo do Transporte = %d\n\n", calculoTransporte(matrizCusto, qtdOrigens + 1, qtdDestinos));
   	}
}

int calculoTransporte(lst_info matriz[][MAX], int lin, int col)
{
	int i, j, total = 0;
	for (i = 0; i < lin; i++) {
		for (j = 0; j < col; j++) {
			total += matriz[i + 1][j + 1].custo * matriz[i + 1][j + 1].custoOtimizado;
		}
	}
	return total;
}

void reiniciaAnaliseEconomica(lst_info matriz[][MAX], int lin, int col)
{
    int i, j;
    for (i = 0; i < lin; i++) {
        for (j = 0; j < col; j++) {
            matriz[i + 1][j + 1].analiseEconomica = 0;
        }
    }
}
void reiniciaDivisaoCustos(ptrNo * vetorOrigens, ptrNo * vetorDestinos, int qtdOrigens, int qtdDestinos)
{
    int i;
    for (i = 0; i < qtdOrigens; i++) vetorOrigens[i + 1]->dado.divisaoCustos = 0;
    for (i = 0; i < qtdDestinos; i++) vetorDestinos[i + 1]->dado.divisaoCustos = 0;
}


void roteamentoMatriz(lst_info matriz[][MAX], int lin, int col, int idlin, int idCol)
{
    int i, j, kLin, kCol;
    bool fleg = false;
    for (j = 0; j < col && fleg != true; j++) {
        if (matriz[idlin][j + 1].custoOtimizado != 0 && (&matriz[idlin][j + 1] != &matriz[idlin][idCol])) {
            kLin = matriz[idlin][j + 1].custoOtimizado;
            for (i = 0; i < lin; i++) {
            	if (matriz[i + 1][idCol].custoOtimizado != 0 && (&matriz[i + 1][idCol] != &matriz[idlin][idCol])) {
            		kCol = matriz[i + 1][idCol].custoOtimizado;
            		matriz[i + 1][idCol].custoOtimizado -= kLin < kCol ? kLin : kCol;
            		matriz[i + 1][j + 1].custoOtimizado += kLin < kCol ? kLin : kCol;
                    matriz[idlin][j + 1].custoOtimizado -= kLin < kCol ? kLin : kCol;
                    matriz[idlin][idCol].custoOtimizado = kLin < kCol ? kLin : kCol;

            		fleg = true;
            		break;
            	}
       		}
    	}
    }
}

void divisaoCustos(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int qtdOrigens, int qtdDestinos)
{
	int i, j, idzero = -1, jdzero = -1;
	for (i = 0; i < qtdOrigens; i++) {
		for (j = 0; j < qtdDestinos; j++) {
			if (matriz[i + 1][j + 1].custoOtimizado != 0) {
				//printf("Analise: \n[%s] %d ; [%s] %d\n", vetorOrigens[i + 1]->dado.name, vetorOrigens[i + 1]->dado.divisaoCustos, vetorDestinos[j + 1]->dado.name, vetorDestinos[j + 1]->dado.divisaoCustos );
				if (vetorOrigens[i + 1]->dado.divisaoCustos == 0 && vetorDestinos[j + 1]->dado.divisaoCustos == 0) {
					if (matriz[i + 1][j + 1].custo != 0) {
                        vetorDestinos[j + 1]->dado.divisaoCustos = matriz[i + 1][j + 1].custo;
					}
					else {
                        idzero = i + 1;
                        jdzero = j + 1;
					}
					//printf("\n[%s] %d\n", vetorDestinos[j + 1]->dado.divisaoCustos);
				}
				else if (vetorOrigens[i + 1]->dado.divisaoCustos == 0 && vetorDestinos[j + 1]->dado.divisaoCustos != 0) {
						vetorOrigens[i + 1]->dado.divisaoCustos = matriz[i + 1][j + 1].custo - vetorDestinos[j + 1]->dado.divisaoCustos;
					}
					else if (vetorDestinos[j + 1]->dado.divisaoCustos == 0 && vetorOrigens[i + 1]->dado.divisaoCustos != 0) {
							vetorDestinos[j + 1]->dado.divisaoCustos = matriz[i + 1][j + 1].custo - vetorOrigens[i + 1]->dado.divisaoCustos;
						}
			}
		}

        if (idzero != -1 || jdzero != -1) {
            if (vetorOrigens[idzero]->dado.divisaoCustos == 0 && vetorDestinos[jdzero]->dado.divisaoCustos == 0) {
                vetorDestinos[jdzero]->dado.divisaoCustos = matriz[idzero][jdzero].custo;
            }
            else if (vetorOrigens[idzero]->dado.divisaoCustos == 0 && vetorDestinos[jdzero]->dado.divisaoCustos != 0) {
                    vetorOrigens[idzero]->dado.divisaoCustos = matriz[idzero][jdzero].custo - vetorDestinos[jdzero]->dado.divisaoCustos;
            }
                else if (vetorDestinos[jdzero]->dado.divisaoCustos == 0 && vetorOrigens[idzero]->dado.divisaoCustos != 0) {
                        vetorDestinos[jdzero]->dado.divisaoCustos = matriz[idzero][jdzero].custo - vetorOrigens[idzero]->dado.divisaoCustos;
                }
        }
        idzero = -1;
        jdzero = -1;
    }
}

void analiseEconomica(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int qtdOrigens, int qtdDestinos)
{
    int i, j;
    for (i = 0; i < qtdOrigens; i++) {
		for (j = 0; j < qtdDestinos; j++) {
			if (matriz[i + 1][j + 1].custoOtimizado == 0) {
                matriz[i + 1][j + 1].analiseEconomica = (vetorOrigens[i + 1]->dado.divisaoCustos + vetorDestinos[j + 1]->dado.divisaoCustos) - matriz[i + 1][j + 1].custo;
            }
        }
    }
}

void metodoMenorCusto(lst_info matrizCusto[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, lst_info * vetMinimos, int qtdOrigens, int qtdDestinos)
{
	int i;
    for (i = 0; i < 	qtdOrigens * qtdDestinos; i++) {
  		if (vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada != 0 && vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada != 0) {

			if (vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada == vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada) {
				matrizCusto[vetMinimos[i].cordenadas.lin][vetMinimos[i].cordenadas.col].custoOtimizado = vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada;
				vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada -= vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada;
				vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada -= vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada;
			}
			else {
				if (vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada < vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada) {
					matrizCusto[vetMinimos[i].cordenadas.lin][vetMinimos[i].cordenadas.col].custoOtimizado = vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada;
					vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada -= vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada;
					vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada -= vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada;
				}
				else {
					matrizCusto[vetMinimos[i].cordenadas.lin][vetMinimos[i].cordenadas.col].custoOtimizado = vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada;
					vetorOrigens[vetMinimos[i].cordenadas.lin]->dado.ofertaDemandaCustomizada -= vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada;
					vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada -= vetorDestinos[vetMinimos[i].cordenadas.col]->dado.ofertaDemandaCustomizada;
				}
			}
   		}
   	}
}

void maiorMatriz(lst_info matriz[][MAX], int lin, int col, int * idLin, int * idCol)
{
    int i, j, maior =  matriz[1][1].analiseEconomica;

    for (i = 0; i < lin; i++) {
        for (j = 0; j < col; j++) {
            if (matriz[i + 1][j + 1].analiseEconomica > maior) {
                *idLin = i + 1;
                *idCol = j + 1;
                maior = matriz[i + 1][j + 1].analiseEconomica;
            }
        }
    }
}

void minimos(lst_info matriz[][MAX], int lin, int col, ptrNo * vetorOrigens, ptrNo * vetorDestinos, lst_info * vetAux)
{
    int  i, j, k = 0;

    for (i = 0; i < lin; i++) {
        for (j = 0; j < col; j++) {
        	matriz[i + 1][j + 1].cordenadas.lin = i + 1;
        	matriz[i + 1][j + 1].cordenadas.col = j + 1;
        	vetAux[k] = matriz[i + 1][j + 1];
			k += 1;
       	}
    }
	insercao(vetAux, lin * col);

}

void vetoresOrigensDestinosInit(ptrNo origens, ptrNo destinos, ptrNo * vetorOrigens, ptrNo * vetorDestino)
{
    int i;

    for (i = 0; origens != NULL; i++) {
        vetorOrigens[i + 1] = origens;
        origens = origens->prox;
    }

    for (i = 0; destinos != NULL; i++) {
        vetorDestino[i + 1] = destinos;
        destinos = destinos->prox;
    }
}


void castMatrizTransporteInit(lst_info matrizCusto[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestino, int lin, int col)
{
    int i, j;
    lst_ptr listDestinos;

    for (i = 0; i < lin; i++) {
        listDestinos = vetorOrigens[i + 1]->dado.destinos;
        while (listDestinos != NULL) {
            for (j = 0; j < col; j++) {
                if (strcmp(listDestinos->dado.name, vetorDestino[j + 1]->dado.name) == 0) {
                        matrizCusto[i + 1][j + 1].custo = listDestinos->dado.custo;
                        break;
                }
            }
            listDestinos = listDestinos->prox;
        }
    }
}

void celulaInfoMatrizInit(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int lin, int col, int totalOferta, int totalDemanda)
{
    int i, j;
    for (i = 0; i < lin; i++) {
        for (j = 0; j < col; j++) {
            if (vetorOrigens[i + 1]->dado.flagTransbordo == true && vetorDestinos[j + 1]->dado.flagTransbordo == true) {
            	matriz[i + 1][j + 1].custo = 0;
            	matriz[i + 1][j + 1].custoOtimizado = 0;
                matriz[i + 1][j + 1].analiseEconomica = 0;
            }
            else {
				 matriz[i + 1][j + 1].custo = overflow; // valor maximo assumil int processador de 16 bits
				 matriz[i + 1][j + 1].custoOtimizado = 0;
				 matriz[i + 1][j + 1].analiseEconomica = 0;
			}
            if (vetorOrigens[i + 1]->dado.flagTransbordo == true || vetorDestinos[j + 1]->dado.flagTransbordo == true) {
                if (vetorOrigens[i + 1]->dado.flagTransbordo) vetorOrigens[i + 1]->dado.ofertaDemanda = (totalDemanda > totalOferta) ? totalDemanda : totalOferta;
                else vetorDestinos[j + 1]->dado.ofertaDemanda = (totalDemanda > totalOferta) ? totalDemanda : totalOferta;
            }
            vetorDestinos[j + 1]->dado.ofertaDemandaCustomizada = vetorDestinos[j + 1]->dado.ofertaDemanda;
            vetorOrigens[i + 1]->dado.ofertaDemandaCustomizada = vetorOrigens[i + 1]->dado.ofertaDemanda;
        }
    }
    //Se for diferente de NULL, siguinifica que existe "outros na"
    if (totalOferta < totalDemanda) {
        //Escreve 0 na linhas "outros"
        for (i = i, j = 0; vetorDestinos[j + 1] != NULL; j++) {
        	matriz[i + 1][j + 1].custo = 0;
        	matriz[i + 1][j + 1].custoOtimizado = 0;
            matriz[i + 1][j + 1].analiseEconomica = 0;
        }
        //Escreve Demandas
        for (i = i + 1, j = 0; strcmp(vetorDestinos[j + 1]->dado.name, "Oferta") != 0; j++) {
        	matriz[i + 1][j + 1].custo = vetorDestinos[j + 1]->dado.ofertaDemanda;
        	matriz[i + 1][j + 1].custoOtimizado = vetorDestinos[j + 1]->dado.ofertaDemanda;
        }
         //Escreve Ofertas
        for (i = 0; vetorOrigens[i + 3] != NULL; i++) {
        	matriz[i + 1][col + 1].custo = vetorOrigens[i + 1]->dado.ofertaDemanda;
        	matriz[i + 1][col + 1].custoOtimizado = vetorOrigens[i + 1]->dado.ofertaDemanda;
        }
         //Escreve diferença oferta e demanda no "outros"
        int diferencaTotalOfertaDemanda = (totalOferta > totalDemanda) ? totalOferta - totalDemanda : totalDemanda - totalOferta;
        matriz[i + 1][col + 1].custo = diferencaTotalOfertaDemanda;
        matriz[i + 1][col + 1].custoOtimizado = diferencaTotalOfertaDemanda;
        matriz[lin + 2][col + 1].custo = 0;
        matriz[lin + 2][col + 1].custoOtimizado = 0;
        matriz[lin + 2][col + 1].analiseEconomica = 0;
    }
    else if(totalOferta > totalDemanda) {
            //Escreve 0 nas linhas coluna dos "outros"
            for (i = 0, j = j; vetorOrigens[i + 1] != NULL; i++) {
            	matriz[i + 1][j + 1].custo = 0;
            	matriz[i + 1][j + 1].custoOtimizado = 0;
                matriz[i + 1][j + 1].analiseEconomica = 0;
            }
            //Escreve Demandas
            for (i = lin, j = 0; strcmp(vetorDestinos[j + 1]->dado.name, "Oferta") != 0; j++) {
            	matriz[i + 1][j + 1].custo = vetorDestinos[j + 1]->dado.ofertaDemanda;
            	matriz[i + 1][j + 1].custoOtimizado = vetorDestinos[j + 1]->dado.ofertaDemanda;
            }
            //Escreve Ofertas
            for (i = 0;  vetorOrigens[i + 2] != NULL; i++) { matriz[i + 1][col + 2].custo = vetorOrigens[i + 1]->dado.ofertaDemanda;
            	matriz[i + 1][col + 2].custoOtimizado = vetorOrigens[i + 1]->dado.ofertaDemanda;
            }
            int diferencaTotalOfertaDemanda = (totalOferta > totalDemanda) ? totalOferta - totalDemanda : totalDemanda - totalOferta;
            matriz[lin + 1][col + 1].custo = diferencaTotalOfertaDemanda;
            matriz[lin + 1][col + 1].custoOtimizado = diferencaTotalOfertaDemanda;
            matriz[lin + 1][col + 2].custo = 0;
            matriz[lin + 1][col + 2].custoOtimizado = 0;
            matriz[lin + 1][col + 2].analiseEconomica = 0;

        }
        else {

            //Escreve Demandas
            for (i = lin, j = 0; strcmp(vetorDestinos[j + 1]->dado.name, "Oferta") != 0; j++) {
            	matriz[i + 1][j + 1].custo = vetorDestinos[j + 1]->dado.ofertaDemanda;
            	matriz[i + 1][j + 1].custoOtimizado = vetorDestinos[j + 1]->dado.ofertaDemanda;
            }
            //Escreve Ofertas
            for (i = 0;  vetorOrigens[i + 2] != NULL; i++) {
            	matriz[i + 1][col + 1].custo = vetorOrigens[i + 1]->dado.ofertaDemanda;
            	matriz[i + 1][col + 1].custoOtimizado = vetorOrigens[i + 1]->dado.ofertaDemanda;
            }
            matriz[lin + 1][col + 1].custo = 0;
            matriz[lin + 1][col + 1].custoOtimizado = 0;
            matriz[lin + 1][col + 1].analiseEconomica = 0;

    }
}

void imprimirMatrizTransporte(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int op, FILE * fptr)
{
    int i, j;
    if (op == 0) {
    	printf("\n[IMPRESSÃO MATRIZ TRANSPORTE]\n\n");
    	fprintf(fptr, "\n[IMPRESSÃO MATRIZ TRANSPORTE]\n\n");
	}
    else if (op == 1) {
	    	printf("\n[IMPRESSÃO MATRIZ TRANSPORTE (MENOR CUSTO/CANTO NOROESTE])\n\n");
	    	fprintf(fptr, "\n[IMPRESSÃO MATRIZ TRANSPORTE (MENOR CUSTO/CANTO NOROESTE])\n\n");
	    }
        else {
        	printf("\n[IMPRESSÃO MATRIZ TRANSPORTE ANALISE ECOMICA]\n\n");
        	fprintf(fptr, "\n[IMPRESSÃO MATRIZ TRANSPORTE ANALISE ECOMICA]\n\n");
        }

    for (i = 0; vetorOrigens[i] != NULL; i++) {
        printf("%-10s\t", vetorOrigens[i]->dado.name);
        fprintf(fptr, "%-10s\t", vetorOrigens[i]->dado.name);
        for (j = 0; vetorDestinos[j + 1] != NULL; j++) {
            if (i == 0) {
            	printf("%-10s\t", vetorDestinos[j + 1]->dado.name);
            	fprintf(fptr, "%-10s\t", vetorDestinos[j + 1]->dado.name);
            }
            else
            	if (op == 0) {
            		printf("%-10d\t", matriz[i][j + 1].custo);
            		fprintf(fptr, "%-10d\t", matriz[i][j + 1].custo);
            	}
            	else
            		if (op == 1) { 
            		 	printf("%-10d\t", matriz[i][j + 1].custoOtimizado);
            		 	fprintf(fptr, "%-10d\t", matriz[i][j + 1].custoOtimizado);
            		}
            		else {
            		 	printf("%-10d\t", matriz[i][j + 1].analiseEconomica);
            		 	fprintf(fptr, "%-10d\t", matriz[i][j + 1].analiseEconomica);
            		}
        }
        printf("\n\n");
       	fprintf(fptr, "\n\n");
    }
}

void imprimirMatrizTransporteDiviCustos(lst_info matriz[][MAX], ptrNo * vetorOrigens, ptrNo * vetorDestinos, int lin, int col, FILE *fptr)
{
	int i;
	printf("\n[IMPRESSÃO MATRIZ TRANSPORTE DE DIVISAO DE CUSTOS]\n\n");
	fprintf(fptr, "\n[IMPRESSÃO MATRIZ TRANSPORTE DE DIVISAO DE CUSTOS]\n\n");

	for (i = 0; i < col; i++) {
        printf("y%d = %d\t", i + 1, vetorDestinos[i + 1]->dado.divisaoCustos);
        fprintf(fptr, "y%d = %d\t", i + 1, vetorDestinos[i + 1]->dado.divisaoCustos);
    }

     printf("\n\n");
     fprintf(fptr, "\n\n");

	for (i = 0; i < lin; i++) {
		printf("x%d = %d\t", i + 1, vetorOrigens[i + 1]->dado.divisaoCustos);
		fprintf(fptr, "x%d = %d\t", i + 1, vetorOrigens[i + 1]->dado.divisaoCustos);
        printf("\n\n");
        fprintf(fptr, "\n\n");
	}
}

void mallocPtrList(lst_ptr * p)
{
    if ((*p = (lst_ptr) malloc(sizeof(lst_ptr))) == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria!\n");
        exit(1);
    }
}

void inicializar_No(ptrNo * l)
{
	*l = NULL;
}

/* Insere o elemento x no final da lista de Nos */
ptrNo inserir_No(ptrNo * l, char name[21])
{
    ptrNo auxNo = lst_findNo(*l, name);
    if (auxNo == NULL) {
        ptrNo n;
        if ((n = (ptrNo) malloc(sizeof(struct lstNo))) == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria[[ptrNo]!\n");
            exit(1);
        }
        strcpy(n->dado.name, name);
        n->dado.flagAdrresExistente = false;
        n->dado.flagTransbordo = false;
        n->dado.ofertaDemanda = 0;
        n->dado.divisaoCustos = 0;
        if (*l == NULL || strcmp(name, (*l)->dado.name) <= 0){
        n->prox = *l;
        *l = n;
        return n;
        }
        else {
            ptrNo p = *l;
            while (p->prox != NULL && strcmp(name,  p->prox->dado.name) > 0) {
                p = p->prox;
            }
            n->prox = p->prox;
            p->prox = n;
            return n;
        }
    }
    return auxNo;
}

/* Verefica se a informação "nome" já existe na lista*/
ptrNo lst_findNo(ptrNo l, char name[21]) {
    while (l != NULL && strcmp(name, l->dado.name) > 0) {
        l = l->prox;
    }
    if (l != NULL && strcmp(name, l->dado.name) == 0) {
        return l;
    }
    return NULL;
}

void imprimir_Nos(ptrNo l, FILE * fptr) {

    printf("\n>[Informacoes Sobre As Rotas Criadas]\n ");
    fprintf(fptr ,"\n>[Informacoes Sobre As Rotas Criadas]\n ");
    while (l != NULL) {
        printf("\n>Origem: ");
        fprintf(fptr, "\n>Origem: ");
        printf("%s\n", l->dado.name);
        fprintf(fptr, "%s\n", l->dado.name);
        printf("<Destinos: ");
        fprintf(fptr, "<Destinos: ");
        lst_print(l->dado.destinos, fptr);
        printf("\n");
        fprintf(fptr, "\n");
        l = l->prox;
    }
    printf("\b\n");
    fprintf(fptr, "\b\n");
}

int sizeListNos(ptrNo l, int * totalOfertaDemanda) {

    int qtdNos = 0;
    *totalOfertaDemanda = 0;
    while (l != NULL) {
        qtdNos++;
        *totalOfertaDemanda += l->dado.ofertaDemanda;
        l = l->prox;
    }

    return qtdNos;
}

void ofertaDemanda(ptrNo l , char strOp[21])
{
    int ofertaDemanda;
    if (strcmp(strOp, "oferta") == 0){
    	printf("\nOrigens\n");
    }
    else {
    	printf("\nDestinos\n");
    }

    while (l != NULL) {
        if (!l->dado.flagTransbordo) {
            printf("%s do %s: ", strOp, l->dado.name);
            scanf("%d", &ofertaDemanda);
            l->dado.ofertaDemanda = ofertaDemanda;
        }
        l = l->prox;
    }
}

void vereficaTransbordos(ptrNo origens, ptrNo destinos)
{
    ptrNo destinosCpy = destinos;
    while (origens != NULL) {
        while (destinos != NULL) {
            if (strcmp(origens->dado.name, destinos->dado.name) == 0) {
                origens->dado.flagTransbordo = true;
                destinos->dado.flagTransbordo = true;
                break;
            }
            destinos = destinos->prox;
        }

        destinos = destinosCpy;
        origens = origens->prox;
    }
}

void insercao(lst_info * v, int tam)
{
	int i = 0;
	int j = 1;
	lst_info aux;
	aux.custo = 0;

	while (j < tam)
	{
		aux = v[j];
		i = j - 1;
		while ((i >= 0) && (v[i].custo > aux.custo))
		{
			v[i + 1] = v[i];
			i = i - 1;
		}
		v[i + 1] = aux;
		j = j + 1;
	}
}
