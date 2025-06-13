#include "VetorFiles.h"
#include <stdio.h>

int main() {
    char* arqTexto[] = {
        "POCs/Beef_Cattle.txt",
        "POCs/Sticker_album.txt"
    };

    int numArq = sizeof(arqTexto) / sizeof(arqTexto[0]);

    VFile VetorTermos;
    InicalizaVetor(&VetorTermos);

    for (int i = 0; i < numArq; i++) {
        FILE* arq = fopen(arqTexto[i], "r");
        if (arq == NULL) continue;
        InsereTermo(&VetorTermos, arq, (i + 1));
        fclose(arq);
    }
    
    ImprimeVetor(VetorTermos);
    LiberaVetor(&VetorTermos);
}