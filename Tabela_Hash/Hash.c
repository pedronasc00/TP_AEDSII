#include "Hash.h"

void THFLVazia(TipoLista *Lista) {
    Lista->Primeiro = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Primeiro;
    Lista->Primeiro->Prox = NULL;
} 

short THVazia(TipoLista Lista) {
    return (Lista.Primeiro == Lista.Ultimo);
}

void Ins(TipoItem x, TipoLista *Lista) {
    Lista->Ultimo->Prox = (TipoCelula *)malloc(sizeof(TipoCelula));
    Lista->Ultimo = Lista->Ultimo->Prox;
    Lista->Ultimo->Item = x;
    Lista->Ultimo->Prox = NULL;
}  

void Ret(TipoApontador p, TipoLista *Lista, TipoItem *Item) {
    TipoApontador q;
    if (THVazia(*Lista) || p == NULL || p->Prox == NULL) {
        printf(" Erro Lista vazia ou posicao nao existe\n");
        return;
    }
    q = p->Prox;
    *Item = q->Item;
    p->Prox = q->Prox;
    if (p->Prox == NULL)
        Lista->Ultimo = p;
    free(q);
}

void GeraPesos(TipoPesos p) {
    int i, j;
    struct timeval semente;
    gettimeofday(&semente, NULL); 
    srand((int)(semente.tv_sec + 1000000 * semente.tv_usec));
    for (i = 0; i < N; i++)
        for (j = 0; j < TAMALFABETO; j++)
            p[i][j] = 1 + (int)(10000.0 * rand() / (RAND_MAX + 1.0));
}

TipoIndice h(TipoChave Chave, TipoPesos p, int M) {
    int i;
    unsigned int Soma = 0; 
    int comp = strlen(Chave);
    for (i = 0; i < comp; i++)
        Soma += p[i][(unsigned int)Chave[i]];
    return (Soma % M);
}

void Inicializa(TipoLista *Tabela, int M) {
    for (int i = 0; i < M; i++)
        THFLVazia(&Tabela[i]);
}

TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M) {
    TipoIndice i = h(Ch, p, M);
    TipoApontador Ap;
    if (THVazia(Tabela[i])) return NULL;
    Ap = Tabela[i].Primeiro;
    while (Ap->Prox->Prox != NULL && strncmp(Ch, Ap->Prox->Item.Chave, sizeof(TipoChave)))
        Ap = Ap->Prox;
    if (!strncmp(Ch, Ap->Prox->Item.Chave, sizeof(TipoChave))) 
        return Ap;
    return NULL;
}

TipoItem* PesquisaItem(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M) {
    TipoApontador pAnterior = Pesquisa(Ch, p, Tabela, M);
    if (pAnterior != NULL)
        return &pAnterior->Prox->Item;
    return NULL;
}  

int Insere(TipoItem x, TipoPesos p, TipoLista *Tabela, int M) {
    if (Pesquisa(x.Chave, p, Tabela, M) == NULL) {
        Ins(x, &Tabela[h(x.Chave, p, M)]);
        return 1; // inseriu nova palavra
    } else {
        // printf("Registro já está presente\n"); // opcional
        return 0; // palavra já existente
    }
}


void Retira(TipoItem x, TipoPesos p, TipoLista *Tabela, int M) {
    TipoApontador Ap = Pesquisa(x.Chave, p, Tabela, M);
    if (Ap == NULL)
        printf(" Registro nao esta presente\n");
    else
        Ret(Ap, &Tabela[h(x.Chave, p, M)], &x);
}

void Imp(TipoLista Lista) {
    TipoApontador Aux = Lista.Primeiro->Prox;
    while (Aux != NULL) {
        printf("Palavra: %s ", Aux->Item.Chave);
        LImprime(&Aux->Item.idPalavra);
        printf("\n");
        Aux = Aux->Prox;
    }
}

void Imprime(TipoLista *Tabela, int M) {
    for (int i = 0; i < M; i++) {
        printf("%d: ", i);
        if (!THVazia(Tabela[i]))
            Imp(Tabela[i]);
        putchar('\n');
    }
} 

void LerPalavra(char *p, int Tam) {
    char c; int i, j = 0;
    fflush(stdin);
    while (((c = getchar()) != '\n') && j < Tam - 1)
        p[j++] = c;
    p[j] = '\0';
    while (c != '\n') c = getchar();
    for (i = j - 1; (i >= 0 && p[i] == ' '); i--)
        p[i] = '\0';
}

void TokenizacaoTermo(char* in, char* out) {
    int c_fim = 0;
    for (int i = 0; in[i] != '\0'; i++) {
        if (isalpha((unsigned char)in[i]))
            out[c_fim++] = tolower((unsigned char)in[i]);
    }
    out[c_fim] = '\0';
}

ApontadorLista BuscaIndice(LLista* listaI, int idDoc) {
    ApontadorLista pAux = listaI->pPrimeiro->pProx;
    while (pAux != NULL) {
        if (pAux->idTermo.idDoc == idDoc)
            return pAux;
        pAux = pAux->pProx;
    }
    return NULL;
}

void ProcessaArquivo(FILE* arq, int idDoc, TipoPesos p, TipoLista *Tabela, int M) {
    if (arq == NULL) return;

    char buffer[100]; 
    TipoChave termoToken;

    while (fscanf(arq, "%99s", buffer) != EOF) {
        TokenizacaoTermo(buffer, termoToken);

        if (strlen(termoToken) == 0) continue;

        TipoItem* itemEncontrado = PesquisaItem(termoToken, p, Tabela, M);

        if (itemEncontrado != NULL) {
            ApontadorLista indiceDoc = BuscaIndice(&itemEncontrado->idPalavra, idDoc);
            if (indiceDoc != NULL) {
                indiceDoc->idTermo.qtde++;
            } else {
                Indice novoIndice = {1, idDoc};
                LInsere(&itemEncontrado->idPalavra, &novoIndice);
            }
        } else {
            TipoItem novoItem;
            strcpy(novoItem.Chave, termoToken);
            FLVazia(&novoItem.idPalavra);

            Indice novoIndice = {1, idDoc};
            LInsere(&novoItem.idPalavra, &novoIndice);

            Insere(novoItem, p, Tabela, M);
            // Não incrementa mais totalPalavrasUnicas aqui!
        }
    }
}

void LiberaTabela(TipoLista *Tabela, int M) {
    for (int i = 0; i < M; i++) {
        TipoApontador pAtual = Tabela[i].Primeiro;
        while (pAtual != NULL) {
            TipoApontador pProximo = pAtual->Prox;
            if (pAtual != Tabela[i].Primeiro) {
                ApontadorLista pIndiceAtual = pAtual->Item.idPalavra.pPrimeiro;
                while (pIndiceAtual != NULL) {
                    ApontadorLista pIndiceProximo = pIndiceAtual->pProx;
                    free(pIndiceAtual);
                    pIndiceAtual = pIndiceProximo;
                }
            }
            free(pAtual);
            pAtual = pProximo;
        }
    }
}