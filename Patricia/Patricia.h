#ifndef PATRICIA_H
#define PATRICIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Hash/Contagem.h"

#define D 8 // numero de bits pra chave

typedef unsigned char TipoChavePatricia;
typedef unsigned int TipoIndexAmp;
typedef unsigned char TipoDib;

typedef enum {
    Interno,
    Externo
} TipoNo;

typedef struct TipoPatNo *TipoArvore;
typedef struct TipoPatNo {
    TipoNo nt;
    union {
        struct {
            TipoIndexAmp Index;
            char caractere;
            TipoArvore Esq, Dir;
        } NInterno;
        struct {
            char *Palavra;
            LLista Ocorrencias; // lista qtde, idDoc
        } NExterno;
    } NO;
} TipoPatNo;

// --- Funções ---

short EExterno(TipoArvore p);
TipoArvore CriaNoint(TipoIndexAmp i, char caractere, TipoArvore Esq, TipoArvore Dir);
TipoArvore CriaNoExt(char *palavra, int idDoc);
void PesquisaPat(TipoArvore t, char *palavra);
TipoArvore InserePat(TipoArvore *raiz, char *palavra, int idDoc);
TipoArvore InsereEntre(char *palavra, int idDoc, TipoArvore *t, TipoIndexAmp i);
void ImprimePatricia(TipoArvore p);
int PrimeiraDiferenca(char *a, char *b);
void LiberaPatricia(TipoArvore t);

#endif