#include "Hash/Hash.h"

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

TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M, int *comparacoes) {
    TipoIndice i = h(Ch, p, M);
    TipoApontador Ap;

    *comparacoes = 0;  // inicia a contagem

    if (THVazia(Tabela[i])) return NULL;

    Ap = Tabela[i].Primeiro;
    while (Ap->Prox->Prox != NULL && strcmp(Ch, Ap->Prox->Item.Chave) != 0) {
        Ap = Ap->Prox;
        (*comparacoes)++; 
    }

    (*comparacoes)++;  // conta a última comparação
    if (strcmp(Ch, Ap->Prox->Item.Chave) == 0)
        return Ap;

    return NULL;
}

TipoItem* PesquisaItem(TipoChave Ch, TipoPesos p, TipoLista *Tabela, int M, int *comparacoes) {
    TipoApontador pAnterior = Pesquisa(Ch, p, Tabela, M, comparacoes);
    if (pAnterior != NULL)
        return &pAnterior->Prox->Item;
    return NULL;
}

int Insere(TipoItem x, TipoPesos p, TipoLista *Tabela, int M, int *totalComparacoes) {
    int comparacoes = 0;
    if (Pesquisa(x.Chave, p, Tabela, M, &comparacoes) == NULL) {
        Ins(x, &Tabela[h(x.Chave, p, M)]);
        *totalComparacoes += comparacoes;
        return 1; // inseriu nova palavra
    } else {
        *totalComparacoes += comparacoes;
        return 0; // palavra já existente
    }
}

void Retira(TipoItem x, TipoPesos p, TipoLista *Tabela, int M) {
    int comparacoes;
    TipoApontador Ap = Pesquisa(x.Chave, p, Tabela, M,&comparacoes);
    if (Ap == NULL)
        printf(" Registro nao esta presente\n");
    else
        Ret(Ap, &Tabela[h(x.Chave, p, M)], &x);
}

void IndiceInvertidoHash(TipoLista* TabelaHash, int M){
    if (TabelaHash == NULL || M == 0) {
        return;
    }
    
    VFile vTermo;
    InicalizaVetor(&vTermo);

    for (int i = 0; i < M; i++) {
        TipoApontador pAux = TabelaHash[i].Primeiro->Prox;
        while (pAux != NULL) {
            vTermo.tamanho++;
            Word* temp_pr = (Word*)realloc(vTermo.VetorF, vTermo.tamanho * sizeof(Word));
            vTermo.VetorF = temp_pr;
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

void ProcessaArquivo(FILE* arq, int idDoc, TipoPesos p, TipoLista *Tabela, int M, int *totalComparacoes) {
    if (arq == NULL) return;
    int comparacoes;
    char buffer[20]; 
    TipoChave termoToken;

    while (fscanf(arq, "%19s", buffer) != EOF) {
        TokenizacaoTermo(buffer, termoToken);

        if (strlen(termoToken) == 0) continue;

        TipoItem* itemEncontrado = PesquisaItem(termoToken, p, Tabela, M,&comparacoes);

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

            Insere(novoItem, p, Tabela, M, totalComparacoes);
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
void PesquisaNaTabelaHash(TipoLista* TabelaHash, int M, TipoPesos p, int *comparacoes) {
    *comparacoes = 0;
    if (TabelaHash == NULL || M == 0) {
        printf("Tabela Hash nao foi criada ainda.\n");
        return;
    }

    char palavraBusca[N];
    printf("Digite a palavra para pesquisa na tabela hash: ");
    scanf("%s", palavraBusca);

    int indiceHash = h(palavraBusca, p, M);
    TipoItem* itemAchado = PesquisaItem(palavraBusca, p, TabelaHash, M, comparacoes);
    //printf("Comparacoes realizadas: %d", *comparacoes);

    if (itemAchado != NULL) {
        printf("\n%d: %s ", indiceHash, itemAchado->Chave);

        ApontadorLista pIndice = itemAchado->idPalavra.pPrimeiro->pProx;
        while (pIndice != NULL) {
            printf("<%d, %d> ", pIndice->idTermo.qtde, (pIndice->idTermo.idDoc)+1);
            pIndice = pIndice->pProx;
        }
        printf("\n");
    } else {
        printf("Palavra '%s' nao encontrada na tabela hash.\n", palavraBusca);
    }
}
