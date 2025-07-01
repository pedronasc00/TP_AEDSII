#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IndiceInvertido/VetorFiles.h"
#include "Hash/Hash.h"
#include "Hash/Contagem.h"
#include "Hash/RelevanciaHASH.h"
#include "Patricia/Compara_Patricia.h"
#include "Patricia/Relevancia_Patricia.h"

// Protótipo da função que lê os arquivos de entrada.
void lerArquivosEntrada(char ***arqTexto_ptr, int *numArqs_ptr);

int main()
{
    char **arqTexto = NULL;
    int numArqs = 0;
    // Carrega a lista de arquivos a serem processados.
    lerArquivosEntrada(&arqTexto, &numArqs);

    // --- Variáveis para a Tabela Hash ---
    TipoLista *TabelaHash = NULL;
    int M = 0;
    TipoPesos p;
    GeraPesos(p); // Gera pesos aleatórios para a função hash.
    int comparacoesInsercaoHash = 0;
    int comparacoesBuscaHash = 0;

    // --- Variáveis para a Árvore Patricia ---
    TipoArvore arvorePatricia = NULL;
    int comparacoesInsercaoPatricia = 0;
    int comparacoesBuscaPatricia = 0;

    int opcao;

    // Loop principal do menu.
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
            // Constrói o índice com Tabela Hash.
            constroiIndiceInvertidoHASH(arqTexto, numArqs, &TabelaHash, &M, p, &comparacoesInsercaoHash);
            break;
        case 2:
            // Constrói o índice com Árvore Patricia.
            constroiIndiceInvertidoPATRICIA(&arvorePatricia, arqTexto, numArqs, &comparacoesInsercaoPatricia);
            break;
        case 3:
            // Busca e calcula relevância com Hash.
            RelevanciaHASH(TabelaHash, M, numArqs, arqTexto, p, &comparacoesBuscaHash);
            break;
        case 4:
            // Busca e calcula relevância com Patricia.
            RelevanciaPatricia(arvorePatricia, arqTexto, numArqs, &comparacoesBuscaPatricia);
            break;
        case 5:
            // Exibe estatísticas de comparação entre as duas estruturas.
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
            // Libera toda a memória alocada antes de sair.
            if (TabelaHash != NULL)
                LiberaTabela(TabelaHash, M);
            if (arvorePatricia != NULL) {
                Patricia_LiberaArvore(&arvorePatricia);
            }
            printf("Saindo...\n");
            break;
        default:
            // Trata opção inválida.
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
    return 0;
}

// Lê o arquivo de entrada que contém a lista dos outros arquivos a serem processados.
void lerArquivosEntrada(char ***arqTexto_ptr, int *numArqs_ptr)
{
    printf("Arquivo de Entrada: ");

    char arq[40];
    scanf("%39s", arq);
    // Abre o arquivo principal que lista os documentos.
    FILE *arqEntrada = fopen(arq, "r");

    if (arqEntrada == NULL)
    {
        return;
    }

    // Lê a quantidade de documentos.
    fscanf(arqEntrada, "%d", numArqs_ptr);
    if (*numArqs_ptr <= 0) // Verificação de segurança
    {
        fclose(arqEntrada);
        return;
    }

    // Aloca memória para o vetor de nomes de arquivos.
    *arqTexto_ptr = (char **)malloc(*numArqs_ptr * sizeof(char *));

    char buffer[50];
    // Lê o nome de cada documento e o armazena no vetor.
    for (int i = 0; i < *numArqs_ptr; i++)
    {
        fscanf(arqEntrada, "%49s", buffer);
        (*arqTexto_ptr)[i] = strdup(buffer); // strdup aloca memória e copia a string.
    }
    fclose(arqEntrada);
}