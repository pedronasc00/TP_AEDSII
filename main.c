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
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                constroiIndiceInvertidoHASH(arqTexto, numArqs, TabelaHash, &M);
                break;
            case 2:
                constroiIndiceInvertidoPATRICIA(&arvorePatricia, arqTexto, numArqs, &comparacoesInsercaoPatricia);
                Patricia_ImprimeArvore(arvorePatricia);
                break;
            case 3:
                ImprimeHashOrdenado(TabelaHash, M);
                break;
            case 4:
                char palavra[32];
                printf("Digite a palavra a ser pesquisada: ");
                scanf("%s", palavra);
                Patricia_Pesquisa(palavra, arvorePatricia, &comparacoesBuscaPatricia);
                break;
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
    printf("=======================================\n");

    return 0;
}
