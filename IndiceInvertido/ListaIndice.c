#include "IndiceInvertido/ListaIndice.h"
#include <stdlib.h> // Adicionado para malloc
#include <stdio.h>  // Adicionado para printf

// Inicializa uma lista encadeada vazia com um nó cabeça (sentinela).
void FLVazia(LLista *tLista)
{
    // O primeiro nó (cabeça) não guarda dados, apenas aponta para o início da lista real.
    tLista->pPrimeiro = (ApontadorLista)malloc(sizeof(LIndice));
    tLista->pUltimo = tLista->pPrimeiro;
    tLista->pPrimeiro->pProx = NULL;
}

// Retorna o número de elementos (nós) na lista.
int LTamanho(LLista *tLista)
{
    int cont = 0;
    ApontadorLista pAux = tLista->pPrimeiro->pProx; // Começa a contar a partir do primeiro elemento real.
    while (pAux != NULL)
    {
        cont++;
        pAux = pAux->pProx;
    }
    return cont;
}

// Insere um novo índice no final da lista de forma eficiente.
int LInsere(LLista *tLista, Indice *pIndice)
{
    // Adiciona o novo nó após o último elemento atual.
    tLista->pUltimo->pProx = (ApontadorLista)malloc(sizeof(LIndice));
    // Atualiza o ponteiro 'pUltimo' para apontar para o novo último nó.
    tLista->pUltimo = tLista->pUltimo->pProx;
    // Copia os dados do índice para o novo nó.
    tLista->pUltimo->idTermo = *pIndice;
    tLista->pUltimo->pProx = NULL; // O novo último nó aponta para NULL.
    return 1;
}

// Imprime todos os elementos da lista no formato <quantidade, id_documento>.
void LImprime(LLista *tLista)
{
    ApontadorLista pAux = NULL;
    pAux = tLista->pPrimeiro->pProx; // Começa a ler a partir do primeiro elemento com dados.
    while (pAux != NULL)
    {
        // Imprime a quantidade de vezes que o termo aparece e o ID do documento.
        printf("<%d, %d> ", pAux->idTermo.qtde, pAux->idTermo.idDoc);
        pAux = pAux->pProx;
    }
}