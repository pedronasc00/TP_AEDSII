#include "VetorFiles.h"

void InicalizaFiles(VFile* vTermo){
    if (vTermo == NULL) return;
    vTermo->VetorF = NULL;
    vTermo->tamanho = 0;
}

void InsereVetorFiles(VFile* vTermo, FILE* arq) {
    if (vTermo == NULL || arq == NULL) return;
    int temp = 0;
    Chave novoTermo;
    
    vTermo->tamanho = ContadorTermo(arq);
    vTermo->VetorF = malloc(vTermo->tamanho * sizeof(novoTermo));

    while (temp < vTermo->tamanho && fscanf(arq, "%19s", novoTermo)) {
        strcpy(vTermo->VetorF[temp].Termo, novoTermo);
        temp++;
    }
}

int ContadorTermo(FILE* arq) {
    int c, tamVetor = 0, flagTermo = 0;

    while ((c = fgetc(arq)) != EOF) {
        
        if (isspace(c)) {
            flagTermo = 0;
        } 
        else if (flagTermo == 0) {
            flagTermo = 1; 
            tamVetor++;   
        }
    }
    rewind(arq);
    return tamVetor;
}

void ImprimeVetor(VFile vfile) {
    if (vfile.VetorF == NULL) return;
    printf("\n--- Conteudo do VFile ---\n");
    printf("Total de palavras: %d\n{ ", vfile.tamanho);
    for (int i = 0; i < vfile.tamanho; i++) {
        printf("%s ", vfile.VetorF[i].Termo);
    }
    printf("}\n-------------------------\n");
}

void LiberaVetor(VFile* vfile) {
    if (vfile == NULL) return;
    free(vfile->VetorF); 
    vfile->VetorF = NULL; 
    vfile->tamanho = 0;   
}