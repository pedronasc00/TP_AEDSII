#include <stdio.h>
#include "VetorFiles.h"
#include "Hash/Hash.h"
#include "Hash/Contagem.h"
#include "Patricia/Patricia.h"
#include "Patricia/Compara_Patricia.h"

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

    int opcao = 0;
    do {
        printf("\n==== Menu ====\n");
        printf("1 - Construir indice invertido (tabela hash)\n");
        printf("2 - Construir indice invertido (PATRICIA)\n");
        printf("3 - Imprimir tabela ordenada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                constroiIndiceInvertidoHASH(arqTexto, numArqs, TabelaHash, &M);
                break;
            case 2:
                constroiIndiceInvertidoPATRICIA();
                break;
            case 3:
                ImprimeHashOrdenado(TabelaHash, M);
                break;
            case 0:
                if (TabelaHash != NULL)
                    LiberaTabela(TabelaHash, M);
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
