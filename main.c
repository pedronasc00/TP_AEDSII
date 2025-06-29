#include <stdio.h>
#include "VetorFiles.h"
#include "Hash/Hash.h"
#include "Hash/Contagem.h"
#include "Patricia/Relevancia_Patricia.h"

int main() {
    char* arqTexto[] = {
        "POCs/ASTSecurer_Vulnerability.txt", 
        "POCs/Beef_Cattle.txt",
        "POCs/Crime_Twitter.txt",
        "POCs/Issue_Tracking.txt"
    };
    int numArqs = sizeof(arqTexto) / sizeof(arqTexto[0]);
    int M = 0;
    TipoLista* TabelaHash = NULL;
    TipoPesos p;
    GeraPesos(p);
    int comparacoes = 0;
    int comparacoesInsercaoHash = 0;


    TipoArvore arvorePatricia = NULL;
    int comparacoesInsercaoPatricia = 0;
    int comparacoesBuscaPatricia = 0;

    int opcao = 0;
    do {
        printf("\n==== Menu ====\n");
        printf("1 - Construir indice invertido (tabela hash)\n");
        printf("2 - Construir indice invertido (PATRICIA)\n");
        printf("3 - Imprimir tabela ordenada\n");
        printf("4 - Pesquisa na Patricia\n");
        printf("5 - Pesquisa na Hash\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                constroiIndiceInvertidoHASH(arqTexto, numArqs, &TabelaHash, &M, p, &comparacoesInsercaoHash);
                break;
            case 2:
                constroiIndiceInvertidoPATRICIA(&arvorePatricia, arqTexto, numArqs, &comparacoesInsercaoPatricia);
                break;
            case 3:
                break;
            case 4:
                /*char palavra[32];
                printf("Digite a palavra a ser pesquisada: ");
                scanf("%s", palavra);
                Patricia_Pesquisa(palavra, arvorePatricia, &comparacoesBuscaPatricia);
                */break;
            case 5:
                PesquisaNaTabelaHash(TabelaHash, M, p, &comparacoes);
                printf("Total de comparacoes da palavra buscada na Hash: %d\n", comparacoes);
                
                if (TabelaHash != NULL) {
                    // 1. Chama a função para obter o vetor de contagens.
                    int* contagens = TamanhoHASH(TabelaHash, M, numArqs);

                    if (contagens != NULL) {
                        // 2. Imprime os resultados.
                        printf("\n---------- Total de Termos por Documento ----------\n");
                        for (int i = 0; i < numArqs; i++) {
                            printf("Documento %d (%s): %d termos\n", i + 1, arqTexto[i], contagens[i]);
                        }
                        printf("---------------------------------------------------\n");
                        
                        // 3. IMPORTANTE: Libera a memória alocada pela função.
                        free(contagens);
                    }
                } else {
                    printf("\nA Tabela Hash ainda não foi construída. Use a opção 1 primeiro.\n");
                }

            case 0:
                if (TabelaHash != NULL)
                    LiberaTabela(TabelaHash, M);
                if (arvorePatricia != NULL){
                    
                    //Isso aqui é so um teste pro calculo da relevancia, se quiser pode apagar ou mudar
                    //Não sei se ta funcionando
                    char *consulta[] = {"the", "beef"};
                    int numTermosConsulta = 2;

                    CalculaRelevanciaPatricia(arvorePatricia, consulta, numTermosConsulta, numArqs);


                    Patricia_LiberaArvore(&arvorePatricia);
                }
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    //Imprime a quantidade de comparações realizadas na Patricia
    printf("\n==== Resultados ====\n");
    printf("Total de comparacoes de insercao na Patricia: %d\n", comparacoesInsercaoPatricia);
    printf("Total de comparacoes de busca na Patricia: %d\n", comparacoesBuscaPatricia);
    //printf("Total de comparacoes de insercao na Patricia: %d\n", comparacoesInsercaoPatricia);
    printf("Total de comparacoes nas insercoes (HASH): %d\n", comparacoesInsercaoHash);

    printf("=======================================\n");

    return 0;
}
