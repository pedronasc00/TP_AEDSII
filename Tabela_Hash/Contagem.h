#ifndef CONTADOR_H
#define CONTADOR_H
#include "Hash.h"
#define FatorCarga 1

int ContaPalavrasUnicas(char* arquivos[], int numArquivos);
int calculaM(int totalPalavrasUnicas, float fatorCarga);
int proximoPrimo(int n);
void constroiIndiceInvertidoHASH();
void imprimirIndiceOrdenado();
#endif
