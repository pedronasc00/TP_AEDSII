#ifndef PATRICIA_H
#define PATRICIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ListaIndice.h"  // Usa sua lista personalizada

#define TAM_MAX_PALAVRA 32
#define D (TAM_MAX_PALAVRA * 8) // Número máximo de bits

typedef char *TipoChavePatricia;
typedef unsigned int TipoIndexAmp;
typedef unsigned char TipoDib;

typedef enum { Interno, Externo } TipoNo;


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
            LLista Ocorrencias; // Usa sua lista de ocorrência
        } NExterno;
    } NO;
} TipoPatNo;



TipoDib Patricia_Bit(TipoIndexAmp i, TipoChavePatricia k, int *comp);
short Patricia_EhExterno(TipoArvore p);

TipoArvore Patricia_CriaNoInterno(int i, TipoArvore Esq, TipoArvore Dir);
TipoArvore Patricia_CriaNoExterno(TipoChavePatricia k, int idDoc);

TipoArvore Patricia_Insere(TipoChavePatricia k, TipoArvore t, int idDoc, int *comp);
void Patricia_Pesquisa(TipoChavePatricia k, TipoArvore t, int *comp);
void IndiceInvertidoPatricia(TipoArvore t, int* cont);
void Patricia_LiberaArvore(TipoArvore *t);

#endif
