#ifndef CONTADOR_H
#define CONTADOR_H
#include "Hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#define FatorCarga 1

int ContaPalavrasUnicas(char* arquivos[], int numArquivos);
int calculaM(int totalPalavrasUnicas, float fatorCarga);
int proximoPrimo(int n);
void constroiIndiceInvertidoHASH(char* arqTexto[], int numArqs, TipoLista **Tabela_ptr, int *M_ptr, TipoPesos p, int *comparacoesInsercaoHash);
int* TamanhoHASH_perDocs(TipoLista* Tabela, int M, int numArqs);


#endif