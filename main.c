#include "VetorFiles.h"

int main() {
    const char* nomesArquivos[] = {
        "POCs/Beef_Cattle.txt",
        "POCs/Sticker_album.txt",
    };

    int numArq = sizeof(nomesArquivos) / sizeof(nomesArquivos[0]);

    VFile VetorTermos;
    InicalizaVetor(&VetorTermos);

    for (int i = 0; i < numArq; i++) {
        FILE* arq = fopen(nomesArquivos[i], "r");
        if (arq == NULL) continue;
        InsereTermo(&VetorTermos, arq, (i + 1));
        fclose(arq);
    }
    
    ImprimeVetor(VetorTermos);
    LiberaVetor(&VetorTermos);
}