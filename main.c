#include "VetorFiles.h"

int main() {
    FILE* arq = fopen("POCs/Beef_Cattle.txt", "r");

    VFile VetorTermo;
    InicalizaVetor(&VetorTermo);
    InsereTermo(&VetorTermo, arq);

    fclose(arq);

    ImprimeVetor(VetorTermo);
    LiberaVetor(&VetorTermo);
}