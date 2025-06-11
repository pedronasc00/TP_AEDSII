#include "VetorFiles.h"

void InicalizaVetor(VFile* vTermo){
    if (vTermo == NULL) return;
    vTermo->VetorF = NULL;
    vTermo->tamanho = 0;
}

void InsereTermo(VFile* vTermo, FILE* arq, int idDoc) {
    if (vTermo == NULL || arq == NULL) return;
    int temp = 0;
    
    Word novoTermo;

    while (temp < vTermo->tamanho && fscanf(arq, "%19s", novoTermo)) {
        TokenizacaoTermo(novoTermo.Palavra, vTermo->VetorF[temp].Palavra);
        temp++;
    }
}

void TokenizacaoTermo(char* in, char* out) {
    int c_fim = 0;
    for (int i = 0; in[i] != '\0'; i++) {
        if (isalpha((unsigned char) in[i])) {
            out[c_fim] = tolower((unsigned char) in[i]);
            c_fim++;
        }
    }
    out[c_fim] = '\0';
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
        printf("%s, ", vfile.VetorF[i].Palavra);
    }
    printf("}\n-------------------------\n");
}

void LiberaVetor(VFile* vfile) {
    if (vfile == NULL) return;
    free(vfile->VetorF); 
    vfile->VetorF = NULL; 
    vfile->tamanho = 0;   
}