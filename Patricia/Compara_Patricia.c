#include "Compara_Patricia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRA 20
typedef char Palavra[MAX_PALAVRA];

void TokenizaPat(char *in, char *out)
{
    int j = 0;
    for (int i = 0; in[i] != '\0'; i++)
    {
        if (isalpha((unsigned char)in[i]))
        {
            out[j++] = tolower((unsigned char)in[i]);
        }
    }
    out[j] = '\0';
}





void constroiIndiceInvertidoPATRICIA()
{
    char *arqTexto[] = {
        "POCs/Beef_Cattle.txt",
        "POCs/Sticker_album.txt"};

    int numArq = sizeof(arqTexto) / sizeof(arqTexto[0]);

    // Inicializa a árvore Patricia vazia
    TipoArvore arvorePatricia = NULL;

    // Processa cada arquivo
    for (int i = 0; i < numArq; i++)
    {
        FILE *arq = fopen(arqTexto[i], "r");
        if (arq == NULL)
        {
            printf("Erro ao abrir %s\n", arqTexto[i]);
            continue;
        }

        char buffer[100];
        char palavraLimpa[100];

        while (fscanf(arq, "%99s", buffer) != EOF)
        {
            TokenizaPat(buffer, palavraLimpa);
            if (strlen(palavraLimpa) == 0)
                continue;

            // Insere a palavra na Patricia, associando ao idDoc (i + 1)
            arvorePatricia = InserePat(&arvorePatricia, palavraLimpa, i + 1);
        }

        fclose(arq);
    }

    // Imprime o índice invertido gerado na Patricia
    printf("\nÍndice Invertido gerado na Patricia:\n\n");
    ImprimePatricia(arvorePatricia);

    // Libera memória
}
