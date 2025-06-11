#ifndef LL_H
#define LL_H
#include "VetorFiles.h"

typedef struct Indice {
    int qtde, idDoc;
}Indice;

typedef struct LIndice *ApontadorLista;
typedef struct LIndice {
    Indice idTermo;
    ApontadorLista pProx;
}LIndice;

typedef struct LLista {
    ApontadorLista pPrimeiro;
    ApontadorLista pUltimo;
}LLista;

void FLVazia(LLista *tLista);
int LTamanho(LLista *tLista);
int LInsere(LLista *tLista, LIndice *pTermo);
void LImprime(LLista *tLista);


#endif