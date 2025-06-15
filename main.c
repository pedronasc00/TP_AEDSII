#include "Hash.h"
#include <stdio.h>

int main() {
    char* arqTexto[] = {
        "POCs/Beef_Cattle.txt",
        "POCs/Sticker_album.txt"
    };

    int numArq = sizeof(arqTexto) / sizeof(arqTexto[0]);

    TipoDicionario Tabela;
    TipoPesos p;
    GeraPesos(p);         // Gera os pesos aleatórios para a função de hash
    Inicializa(Tabela);
    for (int i = 0; i < numArq; i++) {
        FILE* arq = fopen(arqTexto[i], "r");
        if (arq == NULL) continue;
         ProcessaArquivo(arq, (i + 1), p, Tabela);
        fclose(arq);
    }
    
    Imprime(Tabela);
    LiberaTabela(Tabela);
}