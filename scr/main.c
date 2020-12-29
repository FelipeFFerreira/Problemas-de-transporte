#include <stdio.h>
#include <stdlib.h>
#include "transporte.h"

int main()
{
	FILE *ptr_arq;
	ptr_arq = fopen("analise_transporte", "w");
    ptrNo origens;
    ptrNo destinos;

    inicializar_No(&origens);
    inicializar_No(&destinos);

    mallocNos(&origens, &destinos, ptr_arq);
    imprimir_Nos(origens, ptr_arq);

    MatrizTransporteInit(origens, destinos, ptr_arq);
    fclose(ptr_arq);

    return 0 ;
}

