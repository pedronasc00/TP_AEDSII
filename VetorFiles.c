#include "VetorFiles.h"
#include "Hash/Hash.h"

void InicalizaVetor(VFile* vTermo){
    if (vTermo == NULL) return;
    vTermo->VetorF = NULL;
    vTermo->tamanho = 0;
}

int compare(const void *a, const void *b) {
    Word *palavraA = (Word *)a;
    Word *palavraB = (Word *)b;
    return strcmp(palavraA->Palavra, palavraB->Palavra);
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
    
    Indice novoIndice = {1, idDoc};
    
    Chave buffer, termoToken;

    while (fscanf(arq, "%19s", buffer) != EOF) {
        TokenizacaoTermo(buffer, termoToken);
        
        if (strlen(termoToken) == 0) continue; 
        
        Word chaveBusca;
        strcpy(chaveBusca.Palavra, termoToken);
        Word* verifTermo = (Word*)bsearch(&chaveBusca, vTermo->VetorF, vTermo->tamanho, sizeof(Word), compare);

        if (verifTermo != NULL) {
            ApontadorLista idIndice = BuscaIndice(&verifTermo->idPalavra, idDoc);

            if (idIndice != NULL) {
                idIndice->idTermo.qtde++;
            } else {
                LInsere(&verifTermo->idPalavra, &novoIndice);
            }
        } else {
            vTermo->tamanho++;
            
            Word* temp_pr = (Word*)realloc(vTermo->VetorF, vTermo->tamanho * sizeof(Word));

            if (temp_pr == NULL) {
                LiberaVetor(vTermo);
                exit(EXIT_FAILURE);
            }
            
            vTermo->VetorF = temp_pr;

            Word* novoTermo = &vTermo->VetorF[vTermo->tamanho - 1];
            strcpy(novoTermo->Palavra, termoToken);

            FLVazia(&novoTermo->idPalavra);
            LInsere(&novoTermo->idPalavra, &novoIndice);

            qsort(vTermo->VetorF, vTermo->tamanho, sizeof(Word), compare); 
        }
    }
}

void ImprimeVetor(VFile vTermo) {
    if (vTermo.VetorF == NULL) return;
    printf("\n-------------------------\n");
    printf("Total de palavras: %d\n ", vTermo.tamanho);
    for (int i = 0; i < vTermo.tamanho; i++) {
        printf("%s ", vTermo.VetorF[i].Palavra);
        LImprime(&vTermo.VetorF[i].idPalavra);
        printf("\n");
    }
    printf("-------------------------\n");
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

void ImprimeHashOrdenado(TipoLista* TabelaHash, int M){
    if (TabelaHash == NULL || M == 0) {
        return;
    }
    
    VFile vTermo;
    InicalizaVetor(&vTermo);

    for (int i = 0; i < M; i++) {
        TipoApontador pAux = TabelaHash[i].Primeiro->Prox;

        while (pAux != NULL) {
            vTermo.tamanho++;
            vTermo.VetorF = (Word*)realloc(vTermo.VetorF, vTermo.tamanho * sizeof(Word));

            Word* novoTermo = &vTermo.VetorF[vTermo.tamanho - 1];
            strcpy(novoTermo->Palavra, pAux->Item.Chave);
            novoTermo->idPalavra = pAux->Item.idPalavra;

            pAux = pAux->Prox;
        }
    }
    
    qsort(vTermo.VetorF, vTermo.tamanho, sizeof(Word), compare);
    ImprimeVetor(vTermo);
    free(vTermo.VetorF);
}