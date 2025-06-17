#include <stdio.h>
#include "Patricia/Compara_Patricia.h"
int main() {
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
                constroiIndiceInvertidoHASH();
                break;
            case 2:
                constroiIndiceInvertidoPATRICIA();
                break;
            case 3:
                ImprimeVetorOrdenado();
                break;

            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
