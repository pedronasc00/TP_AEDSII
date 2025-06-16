#include "Patricia/Compara_Patricia.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int opcao = 0;
    do
    {
        printf("\n==== Menu ====\n");
        printf("1 - Construir indice invertido (tabela hash)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            break;
        case 0:
            printf("Saindo...\n");
            break;
        case 2:
            constroiIndiceInvertidoPATRICIA();
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
