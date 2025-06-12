#include "VetorFiles.h"

void InicalizaVetor(VFile* vTermo){
    if (vTermo == NULL) return;
    vTermo->VetorF = NULL;
    vTermo->tamanho = 0;
}

Word* BuscarPalavra(VFile* vTermo, char* Termo) {
    for (int i = 0; i < vTermo->tamanho; i++) {
        if (strcmp(vTermo->VetorF[i].Palavra, Termo) == 0) {
            return &vTermo->VetorF[i];
        }
    }
    return NULL;
}

ApontadorLista BuscaIndice(LLista* listaI, int idDoc) {
    ApontadorLista pAux = listaI->pPrimeiro->pProx;
    while (pAux != NULL) {
        if (pAux->idTermo.idDoc == idDoc) {
            return pAux;
        }
        pAux = pAux->pProx;
    }
    return NULL;
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

void InsereTermo(VFile* vTermo, FILE* arq, int idDoc) {
    if (vTermo == NULL || arq == NULL) return;
    int temp = 0;
    
    Chave buffer, termoToken;

    while (fscanf(arq, "%19s", buffer)) {
        TokenizacaoTermo(buffer, termoToken);
        
        if (strlen(termoToken) == 0) continue; 
        
        Indice novoIndice = {1, idDoc};
        Word* VerifTermo = BuscarPalavra(vTermo, termoToken);

        if (VerifTermo != NULL) {
            ApontadorLista idIndice = BuscaIndice(&VerifTermo->idPalavra, idDoc);

            if (idIndice != NULL) {
                idIndice->idTermo.qtde++;
            } else {
                LInsere(&VerifTermo->idPalavra, &novoIndice);
            }
        } else {
            vTermo->tamanho++;
            vTermo->VetorF = (Word*)realloc(vTermo->VetorF, vTermo->tamanho * sizeof(Word));

            Word* novoTermo = &vTermo->VetorF[vTermo->tamanho - 1];
            strcpy(novoTermo->Palavra, termoToken);

            FLVazia(&novoTermo->idPalavra);
            LInsere(&novoTermo->idPalavra, &novoIndice);
        }
    }
}

void ImprimeVetor(VFile vTermo) {
    if (vTermo.VetorF == NULL) return;
    printf("\n--- Conteudo do VFile ---\n");
    printf("Total de palavras: %d\n{ ", vTermo.tamanho);
    for (int i = 0; i < vTermo.tamanho; i++) {
        printf("%s ", vTermo.VetorF[i].Palavra);
        ApontadorLista pAux = vTermo.VetorF[i].idPalavra.pPrimeiro;
        while (pAux != NULL) {
            printf("<%d, %d>\t", pAux->idTermo.qtde, pAux->idTermo.idDoc);
            pAux = pAux->pProx;
        }
        printf("\n");
    }
    printf("}-------------------------\n");
}

void LiberaVetor(VFile* vTermo) {
    if (vTermo == NULL) return;
    for (int i = 0; i < vTermo->tamanho; i++) {
        ApontadorLista temp, pAux = vTermo->VetorF[i].idPalavra.pPrimeiro;
        if (pAux == NULL) continue;
        
        while (pAux != NULL) {
            temp = pAux->pProx;
            free(pAux);
            pAux = temp;
        }
    }
    
    free(vTermo->VetorF); 
    vTermo->VetorF = NULL; 
    vTermo->tamanho = 0;
}