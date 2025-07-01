#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include "IndiceInvertido/VetorFiles.h"

#define N 20
#define TAMALFABETO 256

typedef char TipoChave[N];
typedef unsigned TipoPesos[N][TAMALFABETO]; // Matriz de pesos para a função de hash.

// O item guardado na tabela: a palavra e sua lista de ocorrências.
typedef struct TipoItem {
    TipoChave Chave;
    LLista idPalavra;
} TipoItem;

typedef unsigned int TipoIndice;

typedef struct TipoCelula* TipoApontador;

// Célula (nó) da lista de colisões da tabela hash.
typedef struct TipoCelula {
    TipoItem Item;
    TipoApontador Prox;
} TipoCelula;

// A lista em si, usada para cada entrada da tabela hash.
typedef struct TipoLista {
    TipoCelula *Primeiro, *Ultimo;
} TipoLista;


// Protótipos das funções
void THFLVazia(TipoLista *Lista);
short THVazia(TipoLista Lista);
void Ret(TipoApontador p, TipoLista *Lista, TipoItem *Item);
void GeraPesos(TipoPesos p);
TipoIndice h(TipoChave Chave, TipoPesos p, int M);
void Inicializa(TipoLista *Tabela, int M);
TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M, int *comparacoes);
TipoItem* PesquisaItem(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M, int *comparacoes);
void Ins(TipoItem x, TipoLista *Lista);
int Insere(TipoItem x, TipoPesos p, TipoLista *Tabela, int M, int *totalComparacoes);
void Retira(TipoItem x, TipoPesos p, TipoLista *Tabela, int M);
void IndiceInvertidoHash(TipoLista* TabelaHash, int M);
void LerPalavra(char *p, int Tam);

void ProcessaArquivo(FILE* arq, int idDoc, TipoPesos p, TipoLista *Tabela, int M, int *totalComparacoes);
ApontadorLista BuscaIndice(LLista* listaI, int idDoc);
void LiberaTabela(TipoLista *Tabela, int M);
void PesquisaNaTabelaHash(TipoLista* TabelaHash, char** vetorTermos, int numTermos, int M, TipoPesos p, int* comparacoes);
#endif // HASH_H