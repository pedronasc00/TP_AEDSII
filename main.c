#include "VetorFiles.h"

int main() {
    FILE* arq1 = fopen("POCs/Beef_Cattle.txt", "r");

    VFile VT1;
    InicalizaVetor(&VT1);
    InsereTermo(&VT1, arq1);

    fclose(arq1);

    ImprimeVetor(VT1);

    LiberaVetor(&VT1);
        
    FILE* arq2 = fopen("POCs/Sticker_album.txt", "r");

    VFile VT2;
    InicalizaVetor(&VT2);
    InsereTermo(&VT2, arq2);

    fclose(arq2);

    ImprimeVetor(VT2);

    LiberaVetor(&VT2);
}