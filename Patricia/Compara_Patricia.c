#include "Patricia.h"      // Inclui o cabeçalho da árvore Patricia
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>         // Para tolower, isalnum

#define MAX_PALAVRA 32     // Compatível com a Patricia

// --- Função de Tokenização ---
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

// --- Função que constrói o índice invertido usando Patricia ---
void constroiIndiceInvertidoPATRICIA() {
    // Arquivos de texto para leitura
    char* arqTexto[] = {
        "POCs/ASTSecurer_Vulnerability.txt", 
        "POCs/Beef_Cattle.txt",
        "POCs/Crime_Twitter.txt",
        "POCs/Issue_Tracking.txt"
    };

    int numArq = sizeof(arqTexto) / sizeof(arqTexto[0]);

    // Inicializa a árvore Patricia
    TipoArvore arvorePatricia = NULL;

    for (int i = 0; i < numArq; i++) {
        FILE *arq = fopen(arqTexto[i], "r");
        if (arq == NULL) {
            printf("Erro ao abrir %s\n", arqTexto[i]);
            continue;
        }

        char buffer[100];
        char palavraLimpa[MAX_PALAVRA];

        while (fscanf(arq, "%99s", buffer) != EOF) {
            TokenizacaoTermo_Pat(buffer, palavraLimpa);

            if (strlen(palavraLimpa) == 0) {
                continue;
            }

            // Insere a palavra na Patricia com o id do documento (i+1)
            arvorePatricia = Patricia_Insere(palavraLimpa, arvorePatricia, i + 1);
        }

        fclose(arq);
    }

    // Imprime o índice invertido
    printf("\n--- Indice Invertido Gerado na Patricia ---\n\n");
    Patricia_ImprimeArvore(arvorePatricia);

    // Libera a árvore Patricia
    Patricia_LiberaArvore(&arvorePatricia);
    printf("\nMemoria da arvore Patricia liberada com sucesso.\n");
}
