#ifndef RELEVANCIAHASH_H
#define RELEVANCIAHASH_H
#include "Contagem.h"

typedef struct idRelevancia{
    int idDoc;
    double relevancia;
    char nomeArquivo[100];
}idRelevancia;

int comparaRelevanciaHASH(const void *a, const void *b);
void RelevanciaHASH(TipoLista* Tabela, int M, int numArqs, char* arqTexto[], TipoPesos p, int* comparacoes);

#endif