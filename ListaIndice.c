#include "ListaIndice.h"

void FLVazia(LLista *tLista)
{
    tLista->pPrimeiro = (ApontadorLista)malloc(sizeof(LIndice));
    tLista->pUltimo = tLista->pPrimeiro;
    tLista->pPrimeiro->pProx = NULL;
}

int LTamanho(LLista *tLista)
{
    int cont = 0;
    ApontadorLista pAux = tLista->pPrimeiro->pProx;
    while (pAux != NULL)
    {
        cont++;
        pAux = pAux->pProx;
    }
    return cont;
}

int LInsere(LLista *tLista, Indice *pIndice)
{
    tLista->pUltimo->pProx = (ApontadorLista)malloc(sizeof(LIndice));
    tLista->pUltimo = tLista->pUltimo->pProx;
    tLista->pUltimo->idTermo = *pIndice;
    tLista->pUltimo->pProx = NULL;
    return 1;
}

void LImprime(LLista *tLista)
{
    ApontadorLista pAux = NULL;
    pAux = tLista->pPrimeiro->pProx;
    while (pAux != NULL)
    {
        printf("<%d, %d> ", pAux->idTermo.qtde, pAux->idTermo.idDoc);
        pAux = pAux->pProx;
    }
}
