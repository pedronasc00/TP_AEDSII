#ifndef VFILES_H
#define VFILES_H
#include "ListaIndice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct TipoLista;

typedef char Chave[20];
typedef struct Word {
    Chave Palavra;
    int id;
    LLista idPalavra;
}Word;

typedef struct VFile{
    Word *VetorF;
    int tamanho;
}VFile;

void InicalizaVetor(VFile* vTermo);
void InsereTermo(VFile* vTermo, FILE* arq, int idDoc);
void ImprimeVetor(VFile vTermo);
void LiberaVetor(VFile* vTermo);
void TokenizacaoTermo(char* in, char* out);
int compare(const void *a, const void *b);
ApontadorLista BuscaIndice(LLista* listaI, int idDoc);
#endif