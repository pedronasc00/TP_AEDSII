#ifndef LL_H
#define LL_H

#include <stdio.h>
#include <stdlib.h>

// Armazena a ocorrência: quantidade e ID do documento.
typedef struct Indice
{
    int qtde, idDoc;
} Indice;


typedef struct LIndice *ApontadorLista;

// Nó da lista encadeada.
typedef struct LIndice
{
    Indice idTermo;
    ApontadorLista pProx;
} LIndice;


// Estrutura principal da lista.
typedef struct LLista
{
    ApontadorLista pPrimeiro;
    ApontadorLista pUltimo;
} LLista;


void FLVazia(LLista *tLista);
int LTamanho(LLista *tLista);
int LInsere(LLista *tLista, Indice *pTermo);
void LImprime(LLista *tLista);

#endif