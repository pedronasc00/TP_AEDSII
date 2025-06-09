#ifndef VFILES_H
#define VFILES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef char Chave[20];
typedef struct Word {
    Chave Termo;
}Word;

typedef struct VFile{
    Word *VetorF;
    int tamanho;
}VFile;

void InicalizaVetor(VFile* vTermo);
void InsereTermo(VFile* vtermos, FILE* arq);
void ImprimeVetor(VFile vfile);
int ContadorTermo(FILE* arq);
void LiberaVetor(VFile* vfile);
void TokenizacaoTermo(char* in, char* out);

#endif