#ifndef RELEVANCIA_H
#define RELEVANCIA_H

#include "Compara_Patricia.h"

// Estrutura para armazenar a relevância dos documentos
typedef struct {
    int idDoc;
    double relevancia;
} ResultadoRelevancia;
typedef struct {
    int idDoc;
    double relevancia;
    char nomeArquivo[100];
} ResultadoRelevanciaPatricia;

// Função que calcula e imprime a relevância dos documentos para uma consulta
void CalculaRelevanciaPatricia(
    TipoArvore arvore,
    char **consulta,
    int numTermosConsulta,
    int numDocs
);

// Função auxiliar que conta o número de termos distintos em um documento
int ContaTermosDistintosNoDoc(
    TipoArvore arvore,
    int idDoc
);
void RelevanciaPatricia(TipoArvore arvore, char* arqTexto[], int numDocs);
int comparaRelevanciaPatricia(const void *a,const void *b);
#endif
