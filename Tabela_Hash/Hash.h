#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include "../ListaIndice.h"



#define N 20
#define TAMALFABETO 256

typedef char TipoChave[N];
typedef unsigned TipoPesos[N][TAMALFABETO];

typedef struct TipoItem {
    TipoChave Chave;
    LLista idPalavra;
} TipoItem;

typedef unsigned int TipoIndice;

typedef struct TipoCelula* TipoApontador;
typedef struct TipoCelula {
    TipoItem Item;
    TipoApontador Prox;
} TipoCelula;

typedef struct TipoLista {
    TipoCelula *Primeiro, *Ultimo;
} TipoLista;

// Tabela Hash como ponteiro dinâmico
extern TipoLista* Tabela;  // Deve ser alocada em tempo de execução com malloc

// Protótipos das funções
void THFLVazia(TipoLista *Lista);
short THVazia(TipoLista Lista);
void Ret(TipoApontador p, TipoLista *Lista, TipoItem *Item);
void GeraPesos(TipoPesos p);
TipoIndice h(TipoChave Chave, TipoPesos p, int M);
void Inicializa(TipoLista *Tabela, int M);
TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M);
TipoItem* PesquisaItem(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M);
void Ins(TipoItem x, TipoLista *Lista);
int Insere(TipoItem x, TipoPesos p, TipoLista *Tabela, int M);
void Retira(TipoItem x, TipoPesos p, TipoLista *Tabela, int M);
void Imp(TipoLista Lista);
void Imprime(TipoLista *Tabela, int M);
void LerPalavra(char *p, int Tam);
void TokenizacaoTermo(char* in, char* out);
void ProcessaArquivo(FILE* arq, int idDoc, TipoPesos p, TipoLista *Tabela, int M);
ApontadorLista BuscaIndice(LLista* listaI, int idDoc);
void LiberaTabela(TipoLista *Tabela, int M);

#endif // HASH_H
