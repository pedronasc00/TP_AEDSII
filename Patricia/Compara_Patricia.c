#include "Patricia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRA 32

// Limpa uma palavra, deixando apenas letras/números em minúsculo.
void TokenizacaoTermo_Pat(const char *buffer, char *palavraLimpa) {
    int i = 0, j = 0;
    while (buffer[i] != '\0' && j < MAX_PALAVRA - 1) {
        if (isalnum((unsigned char)buffer[i])) {
            palavraLimpa[j++] = tolower((unsigned char)buffer[i]);
        }
        i++;
    }
    palavraLimpa[j] = '\0';
}


// Cria o índice lendo todos os arquivos de texto.
void constroiIndiceInvertidoPATRICIA(TipoArvore *arvorePatricia, char *arqTexto[], int numArq, int *comparacoesInsercaoPatricia) {
    for (int i = 0; i < numArq; i++) {
        FILE *arq = fopen(arqTexto[i], "r");
        if (arq == NULL) {
            printf("Erro ao abrir %s\n", arqTexto[i]);
            continue;
        }

        char buffer[100];
        char palavraLimpa[32];

        while (fscanf(arq, "%99s", buffer) != EOF) {
            TokenizacaoTermo_Pat(buffer, palavraLimpa);

            if (strlen(palavraLimpa) == 0) continue;

            // Insere a palavra tratada na árvore.
            *arvorePatricia = Patricia_Insere(
                palavraLimpa,
                *arvorePatricia,
                i + 1,
                comparacoesInsercaoPatricia
            );
        }

        fclose(arq);
    }
    printf("\n----------------------\n");
    int cont = 0;
    // Imprime o índice criado.
    IndiceInvertidoPatricia(*arvorePatricia, &cont);
    printf("----------------------\n");
}