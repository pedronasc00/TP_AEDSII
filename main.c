#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VetorFiles.h"
#include "Hash/Hash.h"
#include "Hash/Contagem.h"
#include "Hash/RelevanciaHASH.h"
#include "Patricia/Compara_Patricia.h"
#include "Patricia/Relevancia_Patricia.h"

void lerArquivosEntrada(char ***arqTexto_ptr, int *numArqs_ptr);

int main()
{
    char **arqTexto = NULL;
    int numArqs = 0;
    lerArquivosEntrada(&arqTexto, &numArqs);

    // HASH
    TipoLista *TabelaHash = NULL;
    int M = 0;
    TipoPesos p;
    GeraPesos(p);
    int comparacoesInsercaoHash = 0;
    int comparacoesBuscaHash = 0;

    // PATRICIA
    TipoArvore arvorePatricia = NULL;
    int comparacoesInsercaoPatricia = 0;
    int comparacoesBuscaPatricia = 0;

    int opcao;

    do
    {
        printf("\n==== MENU PRINCIPAL ====\n");
        printf("1 - Construir Indice Invertido (tabela hash)\n");
        printf("2 - Construir Indice Invertido (PATRICIA)\n");
        printf("3 - Pesquisa e Relevancia (HASH)\n");
        printf("4 - Pesquisa e Relevancia (PATRICIA)\n");
        printf("5 - Estatistica de Comparação\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            constroiIndiceInvertidoHASH(arqTexto, numArqs, &TabelaHash, &M, p, &comparacoesInsercaoHash);
            break;
        case 2:
            constroiIndiceInvertidoPATRICIA(&arvorePatricia, arqTexto, numArqs, &comparacoesInsercaoPatricia);
            break;
        case 3:
            RelevanciaHASH(TabelaHash, M, numArqs, arqTexto, p, &comparacoesBuscaHash);
            break;
        case 4:
            RelevanciaPatricia(arvorePatricia, arqTexto, numArqs, &comparacoesBuscaPatricia);
            break;
        case 5:
            if (comparacoesInsercaoHash == 0 || comparacoesInsercaoPatricia == 0)
            {
                printf("Erro: Construa os Indices da HASH e PATRICIA\n");
                break;
            }
            printf("\n==== Estatisticas de Comparacao ====\n");
            printf("Tabela Hash:\n");
            printf("Numero de comparacoes na inserção (HASH): %d\n", comparacoesInsercaoHash);
            printf("Numero de comparacoes na busca (HASH): %d\n", comparacoesBuscaHash);
            printf("\n");
            printf("Tabela PATRICIA:\n");
            printf("Numero de comparacoes na inserção (PATRICIA): %d\n", comparacoesInsercaoPatricia);
            printf("Numero de comparacoes na busca (PATRICIA): %d\n", comparacoesBuscaPatricia);
        case 0:
            if (TabelaHash != NULL)
                LiberaTabela(TabelaHash, M);
            if (arvorePatricia != NULL) {
                Patricia_LiberaArvore(&arvorePatricia);
            }
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
    return 0;
}

void lerArquivosEntrada(char ***arqTexto_ptr, int *numArqs_ptr)
{
    printf("Arquivo de Entrada: ");

    char arq[40];
    scanf("%39s", arq);
    FILE *arqEntrada = fopen(arq, "r");

    if (arqEntrada == NULL)
    {
        return;
    }

    fscanf(arqEntrada, "%d", numArqs_ptr);
    if (*numArqs_ptr < 0 || numArqs_ptr == NULL)
    {
        return;
    }

    *arqTexto_ptr = (char **)malloc(*numArqs_ptr * sizeof(char *));

    char buffer[50];
    for (int i = 0; i < *numArqs_ptr; i++)
    {
        fscanf(arqEntrada, "%49s", buffer);
        (*arqTexto_ptr)[i] = strdup(buffer);
    }
    fclose(arqEntrada);
}